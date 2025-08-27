#include "FileController.h"
#include "FileTableModel.h"

#include <QDirIterator>
#include <QFileInfo>
#include <QDateTime>
#include <QMetaObject>
#include <QThread>

void FileController::setSelectedFolderFromQml(const QUrl& url)
{
    if (!m_model) return;
    const QString path = url.isLocalFile() ? url.toLocalFile()
        : url.toString();  // fallback
    m_model->setSelectedFolder(path);
}

static QString classifyBySuffix(const QString& suffixLower)
{
    // minimal, change as you wish
    static const QStringList img = { "png","jpg","jpeg","gif","bmp","webp","tif","tiff" };
    static const QStringList vid = { "mp4","mov","mkv","avi","webm" };
    static const QStringList doc = { "pdf","doc","docx","txt","rtf" };
    static const QStringList xls = { "xls","xlsx","csv" };
    if (img.contains(suffixLower)) return "Image";
    if (vid.contains(suffixLower)) return "Video";
    if (doc.contains(suffixLower)) return "Document";
    if (xls.contains(suffixLower)) return "Excel";
    return suffixLower.isEmpty() ? "File" : suffixLower;
}

void FileController::scan()
{
    if (!m_model) return;
    const QString root = m_model->selectedFolder();
    if (root.isEmpty()) return;

    // If a previous scan thread exists, clean it up first
    if (m_scanThread) {
        if (m_scanThread->isRunning()) {
            // optional: you could signal cancellation; here we just wait briefly
            m_scanThread->quit();
            m_scanThread->wait();
        }
        m_scanThread->deleteLater();
        m_scanThread = nullptr;
    }

    // Mark scanning active before starting
    m_scanning = true;                  
    emit scanningChanged();             

    // Launch a dedicated worker thread using QThread::create (Qt 6)
    m_scanThread = QThread::create([this, root]() {
        // Clear model on GUI thread before filling (comment out if you prefer appending)
        QMetaObject::invokeMethod(m_model, "clear", Qt::QueuedConnection);

        QDirIterator it(root, QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext()) {

            // Check if was send stop request
            if (QThread::currentThread()->isInterruptionRequested()) { break; }
                
            it.next();
            const QFileInfo fi = it.fileInfo();

            const QString name = fi.fileName();
            const quint64 size = static_cast<quint64>(fi.size());
            const QString type = classifyBySuffix(fi.suffix().toLower());
            const QString date = fi.lastModified().toString("dd.MM.yy");

            // Marshal addFile to the model's (GUI) thread
            QMetaObject::invokeMethod(
                m_model,
                [this, name, size, type, date]() {
                    m_model->addFile(name, size, type, date);
                },
                Qt::QueuedConnection);

            // Artificial pause to demonstrate non-blocking UI
            QThread::msleep(500);
        }
        });

    // Auto-cleanup when done
    connect(m_scanThread, &QThread::finished, m_scanThread, &QObject::deleteLater);
    connect(m_scanThread, &QThread::finished, this, [this]() 
        {
            m_scanThread = nullptr; 
            m_scanning = false;              
            emit scanningChanged();         
        });

    m_scanThread->start();
}

void FileController::stop()
{
    // Tell the worker thread to stop; it will exit when the loop checks this.
    if (m_scanThread && m_scanThread->isRunning())
        m_scanThread->requestInterruption();
}
#include "FileController.h"
#include "FileTableModel.h"

#include <QDirIterator>
#include <QFileInfo>
#include <QDateTime>
#include <QMetaObject>
#include <QThread>
#include <QUrl>

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

    // stop/cleanup previous worker if any
    if (m_scanThread) {                         // QPointer: true only if object still alive
        QThread* t = m_scanThread.data();       // raw ptr for clarity
        if (t->isRunning()) {
            t->requestInterruption();
            t->quit();
            t->wait();
        }
        delete t;                               // safe now (not running)
        m_scanThread.clear();                   // ensure nullptr
    }

    m_scanning = true;
    emit scanningChanged();

    // start timer & reset last duration
    m_scanTimer.restart();                   
    m_lastScanDurationMs = 0;                 
    emit scanDurationChanged();               

    m_scanThread = QThread::create([this, root]() {
        // pre-count for percent
        const qsizetype totalFiles = countFiles(root);

        // reset progress (& optionally clear the model)
        resetProgressAsync();
        QMetaObject::invokeMethod(m_model, "clear", Qt::QueuedConnection);

        qsizetype processed = 0;
        int        found = 0;
        qulonglong bytes = 0;

        QDirIterator it(root, QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            if (QThread::currentThread()->isInterruptionRequested())
                break;

            it.next();
            const QFileInfo fi = it.fileInfo();
            const QString name = fi.fileName();
            const quint64 size = static_cast<quint64>(fi.size());
            const QString type = classifyBySuffix(fi.suffix().toLower());
            const QString date = fi.lastModified().toString("dd.MM.yy");

            // add row on GUI thread
            QMetaObject::invokeMethod(m_model, [this, name, size, type, date]() {
                m_model->addFile(name, size, type, date);
                }, Qt::QueuedConnection);

            // progress snapshot
            ++processed; ++found; bytes += size;
            const int percent = (totalFiles > 0)
                ? int((100.0 * processed) / totalFiles + 0.5) : 0;
            updateProgressAsync(percent, found, bytes);

            // artificial pause to demonstrate non-blocking UI
            QThread::msleep(500);
        }

        finalizeProgressAsync();
        });

    connect(m_scanThread, &QThread::finished, this, [this]() {
        m_scanning = false;
        emit scanningChanged();

        m_lastScanDurationMs = m_scanTimer.elapsed();   
        emit scanDurationChanged();                     
    });
    connect(m_scanThread, &QThread::finished, m_scanThread, &QObject::deleteLater);

    m_scanThread->start();
}

void FileController::stop()
{
    // Tell the worker thread to stop; it will exit when the loop checks this.
    if (m_scanThread && m_scanThread->isRunning())
        m_scanThread->requestInterruption();
}

void FileController::resetProgressAsync()
{
    QMetaObject::invokeMethod(this, [this]() {
        m_progressPercent = 0;
        m_foundCount = 0;
        m_totalSizeBytes = 0;
        emit progressChanged();
        }, Qt::QueuedConnection);
}

void FileController::updateProgressAsync(int percent, int found, qulonglong bytes)
{
    QMetaObject::invokeMethod(this, [this, percent, found, bytes]() {
        m_progressPercent = percent;
        m_foundCount = found;
        m_totalSizeBytes = bytes;
        emit progressChanged();
        }, Qt::QueuedConnection);
}

void FileController::finalizeProgressAsync()
{
    QMetaObject::invokeMethod(this, [this]() {
        if (m_progressPercent < 100) {
            m_progressPercent = 100;
            emit progressChanged();
        }
        }, Qt::QueuedConnection);
}

qsizetype FileController::countFiles(const QString& root)
{
    qsizetype total = 0;
    for (QDirIterator it(root, QDir::Files, QDirIterator::Subdirectories); it.hasNext();) {
        if (QThread::currentThread()->isInterruptionRequested())
            break;
        it.next();
        ++total;
    }
    return total;
}

bool FileController::scanning() const
{
    return m_scanning;
}

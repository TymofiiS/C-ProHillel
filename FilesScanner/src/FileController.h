#pragma once
#include <QObject>
#include <QThread>
#include <QUrl>
#include <QPointer>

class FileTableModel;

class FileController : public QObject
{
    Q_OBJECT
        Q_PROPERTY(bool        scanning        READ scanning        NOTIFY scanningChanged)
        Q_PROPERTY(int         progressPercent READ progressPercent NOTIFY progressChanged)
        Q_PROPERTY(int         foundCount      READ foundCount      NOTIFY progressChanged)
        Q_PROPERTY(qulonglong  totalSizeBytes  READ totalSizeBytes  NOTIFY progressChanged)

public:
    explicit FileController(FileTableModel* model, QObject* parent = nullptr)
        : QObject(parent), m_model(model) {
    }

    Q_INVOKABLE void scan();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void setSelectedFolderFromQml(const QUrl& url);

    // Declarations only 
    bool scanning() const;                
    int  progressPercent() const { return m_progressPercent; }
    int  foundCount() const { return m_foundCount; }
    qulonglong totalSizeBytes() const { return m_totalSizeBytes; }

signals:
    void scanningChanged();
    void progressChanged();

private:
    // progress helpers 
    void resetProgressAsync();
    void updateProgressAsync(int percent, int found, qulonglong bytes);
    void finalizeProgressAsync();
    qsizetype countFiles(const QString& root);

    FileTableModel* m_model = nullptr;
    QPointer<QThread> m_scanThread;

    // Keep ONE member definition
    bool        m_scanning = false;  
    int         m_progressPercent = 0;
    int         m_foundCount = 0;
    qulonglong  m_totalSizeBytes = 0;
};

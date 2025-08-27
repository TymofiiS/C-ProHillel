
#pragma once
#include <QObject>

class FileController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int progressPercent READ progressPercent WRITE setProgressPercent NOTIFY progressChanged)
    Q_PROPERTY(int foundCount READ foundCount WRITE setFoundCount NOTIFY progressChanged)
    Q_PROPERTY(qulonglong totalSizeBytes READ totalSizeBytes WRITE setTotalSizeBytes NOTIFY progressChanged)

public:
    explicit FileController(QObject* parent = nullptr) : QObject(parent) {}

    // --- Placeholders for UI buttons ---
    Q_INVOKABLE void chooseFolder() { /* TODO: placeholder */ }
    Q_INVOKABLE void scan()         { /* TODO: placeholder */ }
    Q_INVOKABLE void stop()         { /* TODO: placeholder */ }
    Q_INVOKABLE void exportData()   { /* TODO: placeholder */ }

    // --- Minimal API to drive the progress block (placeholders) ---
    int progressPercent() const { return m_progress; }
    int foundCount() const { return m_foundCount; }
    qulonglong totalSizeBytes() const { return m_totalSize; }

    Q_INVOKABLE void setProgressPercent(int p) {
        if (p < 0) p = 0; if (p > 100) p = 100;
        if (m_progress == p) return;
        m_progress = p; emit progressChanged();
    }
    Q_INVOKABLE void setFoundCount(int c) {
        if (c < 0) c = 0;
        if (m_foundCount == c) return;
        m_foundCount = c; emit progressChanged();
    }
    Q_INVOKABLE void setTotalSizeBytes(qulonglong bytes) {
        if (m_totalSize == bytes) return;
        m_totalSize = bytes; emit progressChanged();
    }

signals:
    void progressChanged();

private:
    int m_progress = 65;             // demo default to match screenshot
    int m_foundCount = 1234;         // demo default
    qulonglong m_totalSize = 2300000000ULL; // 2.3 GB demo
};

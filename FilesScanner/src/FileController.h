
#pragma once
#include <QObject>
#include <QUrl>
#include <QThread>  

class FileTableModel;

class FileController : public QObject
{
    Q_OBJECT
public:
    explicit FileController(FileTableModel* model, QObject* parent = nullptr)
        : QObject(parent), m_model(model) {
    }

    // Placeholders
    Q_INVOKABLE void scan();
    Q_INVOKABLE void stop();
    Q_PROPERTY(bool scanning READ scanning NOTIFY scanningChanged);

    bool scanning() const { return m_scanning; }

    // Called from QML FolderDialog
    Q_INVOKABLE void setSelectedFolderFromQml(const QUrl& url);

signals:
    void scanningChanged();

private:
    FileTableModel* m_model = nullptr; // not owned
    QThread* m_scanThread = nullptr;
    bool m_scanning = false;
};

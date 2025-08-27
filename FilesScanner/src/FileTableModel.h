
#pragma once
#include <QAbstractTableModel>
#include <vector>
#include <unordered_map>

struct FileRow {
    QString name;
    quint64 sizeBytes = 0;
    QString type;
    QString dateDisplay;
};

class FileTableModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(int         totalCount            READ totalCount            NOTIFY statsChanged)
    Q_PROPERTY(qulonglong  totalSizeBytes        READ totalSizeBytes        NOTIFY statsChanged)
    Q_PROPERTY(QString     largestFileName       READ largestFileName       NOTIFY statsChanged)
    Q_PROPERTY(qulonglong  largestFileSize       READ largestFileSize       NOTIFY statsChanged)
    Q_PROPERTY(QString     mostCommonType        READ mostCommonType        NOTIFY statsChanged)
    Q_PROPERTY(int         mostCommonTypePercent READ mostCommonTypePercent NOTIFY statsChanged)
    Q_PROPERTY(QString     selectedFolder        READ selectedFolder        NOTIFY selectedFolderChanged)

public:
    enum Columns { NameCol = 0, SizeCol, TypeCol, DateCol, ColumnCount };
    enum Roles { NameRole = Qt::UserRole + 1, SizeBytesRole, TypeRole, DateRole };

    explicit FileTableModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void addFile(const QString& name, quint64 sizeBytes, const QString& type, const QString& dateDisplay);
    Q_INVOKABLE void clear();

    // Stats getters
    int         totalCount() const { return static_cast<int>(m_rows.size()); }
    qulonglong  totalSizeBytes() const { return m_totalSize; }
    QString     largestFileName() const { return m_largestName; }
    qulonglong  largestFileSize() const { return m_largestSize; }
    QString     mostCommonType() const { return m_commonType; }
    int         mostCommonTypePercent() const { return m_commonPercent; }

    // Folder selection
    QString selectedFolder() const { return m_selectedFolder; }
    void setSelectedFolder(const QString& folder);

signals:
    void statsChanged();
    void selectedFolderChanged();

private:
    void recomputeStats();

private:
    std::vector<FileRow> m_rows;
    qulonglong m_totalSize = 0;
    QString    m_largestName;
    qulonglong m_largestSize = 0;
    QString    m_commonType;
    int        m_commonPercent = 0;
    QString    m_selectedFolder;
};

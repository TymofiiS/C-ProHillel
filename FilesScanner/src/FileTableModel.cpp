
#include "FileTableModel.h"

FileTableModel::FileTableModel(QObject* parent) : QAbstractTableModel(parent) {}

int FileTableModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid()) return 0;
    return static_cast<int>(m_rows.size());
}

int FileTableModel::columnCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return ColumnCount;
}

QVariant FileTableModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= rowCount()) return {};
    const auto& r = m_rows[static_cast<size_t>(index.row())];
    switch (role) {
    case NameRole:      return r.name;
    case SizeBytesRole: return QVariant::fromValue<qulonglong>(r.sizeBytes);
    case TypeRole:      return r.type;
    case DateRole:      return r.dateDisplay;
    default:
        if (role == Qt::DisplayRole) {
            switch (index.column()) {
            case NameCol: return r.name;
            case SizeCol: return QVariant::fromValue<qulonglong>(r.sizeBytes);
            case TypeCol: return r.type;
            case DateCol: return r.dateDisplay;
            default: return {};
            }
        }
        return {};
    }
}

QHash<int, QByteArray> FileTableModel::roleNames() const {
    return {
        { NameRole, "name" },
        { SizeBytesRole, "sizeBytes" },
        { TypeRole, "type" },
        { DateRole, "dateDisplay" }
    };
}

void FileTableModel::addFile(const QString& name, quint64 sizeBytes, const QString& type, const QString& dateDisplay) {
    const int row = rowCount();
    beginInsertRows(QModelIndex(), row, row);
    m_rows.push_back(FileRow{ name, sizeBytes, type, dateDisplay });
    endInsertRows();
    recomputeStats();
}

void FileTableModel::setSelectedFolder(const QString& folder) {
    if (m_selectedFolder == folder) return;
    m_selectedFolder = folder;
    emit selectedFolderChanged();
}

void FileTableModel::recomputeStats() {
    m_totalSize = 0;
    m_largestSize = 0;
    m_largestName.clear();
    m_commonType.clear();
    m_commonPercent = 0;

    std::unordered_map<QString, int> freq;
    for (const auto& r : m_rows) {
        m_totalSize += r.sizeBytes;
        if (r.sizeBytes > m_largestSize) {
            m_largestSize = r.sizeBytes;
            m_largestName = r.name;
        }
        freq[r.type]++;
    }

    int best = 0;
    QString bestType;
    for (const auto& kv : freq) {
        if (kv.second > best) { best = kv.second; bestType = kv.first; }
    }
    m_commonType = bestType;
    m_commonPercent = (m_rows.empty() || best == 0) ? 0 : int((100.0 * best) / m_rows.size() + 0.5);

    emit statsChanged();
}

void FileTableModel::clear()
{
    beginResetModel();
    m_rows.clear();
    endResetModel();
    recomputeStats();
}
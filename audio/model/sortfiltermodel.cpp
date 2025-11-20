#include "sortfiltermodel.h"
#include <QQmlContext>
#include <QQmlEngine>
#include <QDebug>

SortFilterModel::SortFilterModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
    connect(this, &QAbstractItemModel::rowsInserted, this, &SortFilterModel::countChanged);
    connect(this, &QAbstractItemModel::rowsRemoved, this, &SortFilterModel::countChanged);
    connect(this, &QAbstractItemModel::modelReset, this, &SortFilterModel::countChanged);
}

SortFilterModel::~SortFilterModel()
{
}

QAbstractItemModel *SortFilterModel::sourceModel() const
{
    return QSortFilterProxyModel::sourceModel();
}

void SortFilterModel::setModel(QAbstractItemModel *source)
{
    if (source == QSortFilterProxyModel::sourceModel()) {
        return;
    }

    if (QSortFilterProxyModel::sourceModel()) {
        disconnect(QSortFilterProxyModel::sourceModel(), &QAbstractItemModel::modelReset, this, &SortFilterModel::syncRoleNames);
    }

    QSortFilterProxyModel::setSourceModel(source);

    if (source) {
        connect(source, &QAbstractItemModel::modelReset, this, &SortFilterModel::syncRoleNames);
        syncRoleNames();
    }

    Q_EMIT sourceModelChanged(source);
}

void SortFilterModel::setFilterRegularExpression(const QString &pattern)
{
    if (pattern == filterRegularExpression()) {
        return;
    }
    
    m_filterRegex.setPattern(pattern);
    m_filterRegex.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
    QSortFilterProxyModel::setFilterRegularExpression(m_filterRegex);
    Q_EMIT filterRegularExpressionChanged(pattern);
}

QString SortFilterModel::filterRegularExpression() const
{
    return QSortFilterProxyModel::filterRegularExpression().pattern();
}

void SortFilterModel::setFilterString(const QString &filterString)
{
    if (filterString == m_filterString) {
        return;
    }
    m_filterString = filterString;
    QSortFilterProxyModel::setFilterFixedString(filterString);
    Q_EMIT filterStringChanged(filterString);
}

QString SortFilterModel::filterString() const
{
    return m_filterString;
}

QJSValue SortFilterModel::filterCallback() const
{
    return m_filterCallback;
}

void SortFilterModel::setFilterCallback(const QJSValue &callback)
{
    if (m_filterCallback.strictlyEquals(callback)) {
        return;
    }

    if (!callback.isNull() && !callback.isCallable()) {
        return;
    }

    m_filterCallback = callback;
    invalidateFilter();
    Q_EMIT filterCallbackChanged(callback);
}

void SortFilterModel::setFilterRole(const QString &role)
{
    if (role == m_filterRole) {
        return;
    }
    m_filterRole = role;
    QSortFilterProxyModel::setFilterRole(roleNameToId(role));
    Q_EMIT filterRoleChanged(role);
}

QString SortFilterModel::filterRole() const
{
    return m_filterRole;
}

void SortFilterModel::setSortRole(const QString &role)
{
    if (role == m_sortRole) {
        return;
    }
    m_sortRole = role;
    if (role.isEmpty()) {
        sort(-1, Qt::AscendingOrder);
    } else if (sourceModel()) {
        QSortFilterProxyModel::setSortRole(roleNameToId(role));
        sort(sortColumn(), sortOrder());
    }
    Q_EMIT sortRoleChanged(role);
}

QString SortFilterModel::sortRole() const
{
    return m_sortRole;
}

Qt::SortOrder SortFilterModel::sortOrder() const
{
    return QSortFilterProxyModel::sortOrder();
}

void SortFilterModel::setSortOrder(Qt::SortOrder order)
{
    if (order == sortOrder()) {
        return;
    }
    sort(sortColumn(), order);
    Q_EMIT sortOrderChanged();
}

int SortFilterModel::sortColumn() const
{
    return QSortFilterProxyModel::sortColumn();
}

void SortFilterModel::setSortColumn(int column)
{
    if (column == sortColumn()) {
        return;
    }
    sort(column, sortOrder());
    Q_EMIT sortColumnChanged();
}

QVariantMap SortFilterModel::get(int row) const
{
    QModelIndex idx = index(row, 0);
    QVariantMap hash;

    const QHash<int, QByteArray> rNames = roleNames();
    for (auto i = rNames.begin(); i != rNames.end(); ++i) {
        hash[QString::fromUtf8(i.value())] = data(idx, i.key());
    }

    return hash;
}

int SortFilterModel::mapRowToSource(int row) const
{
    QModelIndex idx = index(row, 0);
    return mapToSource(idx).row();
}

int SortFilterModel::mapRowFromSource(int row) const
{
    if (!sourceModel()) {
        qWarning() << "No source model defined!";
        return -1;
    }
    QModelIndex idx = sourceModel()->index(row, 0);
    return mapFromSource(idx).row();
}

bool SortFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if (m_filterCallback.isCallable()) {
        QJSValueList args;
        args << QJSValue(source_row);

        const QModelIndex idx = sourceModel()->index(source_row, filterKeyColumn(), source_parent);
        QQmlEngine *engine = QQmlEngine::contextForObject(this)->engine();
        args << engine->toScriptValue<QVariant>(idx.data(roleNameToId(m_filterRole)));

        return const_cast<SortFilterModel *>(this)->m_filterCallback.call(args).toBool();
    }

    return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
}

QHash<int, QByteArray> SortFilterModel::roleNames() const
{
    if (sourceModel()) {
        return sourceModel()->roleNames();
    }
    return {};
}

void SortFilterModel::syncRoleNames()
{
    if (!sourceModel()) {
        return;
    }

    m_roleIds.clear();
    const QHash<int, QByteArray> rNames = roleNames();
    m_roleIds.reserve(rNames.count());
    for (auto i = rNames.constBegin(); i != rNames.constEnd(); ++i) {
        m_roleIds[QString::fromUtf8(i.value())] = i.key();
    }

    // 更新过滤和排序角色
    if (!m_filterRole.isEmpty()) {
        QSortFilterProxyModel::setFilterRole(roleNameToId(m_filterRole));
    }
    if (!m_sortRole.isEmpty()) {
        QSortFilterProxyModel::setSortRole(roleNameToId(m_sortRole));
    }
}

int SortFilterModel::roleNameToId(const QString &name) const
{
    return m_roleIds.value(name, -1);
}
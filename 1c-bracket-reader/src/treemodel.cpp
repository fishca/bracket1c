#include "treemodel.h"

TreeModel::TreeModel(QObject *parent)
    : QAbstractItemModel(parent)
    , mRootNode(nullptr)
{
}

TreeModel::~TreeModel() = default;

void TreeModel::setRootNode(std::unique_ptr<TreeNode> root)
{
    beginResetModel();
    mRootNode = std::move(root);
    updateFullPaths();
    endResetModel();
}

TreeNode *TreeModel::nodeFromIndex(const QModelIndex &index) const
{
    if (!index.isValid())
        return mRootNode.get();
    
    return static_cast<TreeNode*>(index.internalPointer());
}

void TreeModel::updateFullPaths()
{
    if (mRootNode) {
        updateFullPathsRecursive(mRootNode.get(), QString());
    }
}

void TreeModel::updateFullPathsRecursive(TreeNode *node, const QString &parentPath)
{
    if (!node)
        return;
    
    QString currentPath = parentPath.isEmpty() ? node->data() 
                                               : parentPath + " / " + node->data();
    node->setFullPath(currentPath);
    
    for (int i = 0; i < node->childCount(); ++i) {
        updateFullPathsRecursive(node->child(i), currentPath);
    }
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    TreeNode *item = static_cast<TreeNode*>(index.internalPointer());
    return item->data();
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return tr("Узел");

    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeNode *parentItem;

    if (!parent.isValid())
        parentItem = mRootNode.get();
    else
        parentItem = static_cast<TreeNode*>(parent.internalPointer());

    TreeNode *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeNode *childItem = static_cast<TreeNode*>(index.internalPointer());
    TreeNode *parentItem = childItem->parent();

    if (parentItem == mRootNode.get())
        return QModelIndex();

    if (parentItem)
        return createIndex(parentItem->row(), 0, parentItem);
    
    return QModelIndex();
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeNode *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = mRootNode.get();
    else
        parentItem = static_cast<TreeNode*>(parent.internalPointer());

    if (!parentItem)
        return 0;
        
    return parentItem->childCount();
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}

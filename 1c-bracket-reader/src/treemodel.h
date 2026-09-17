#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <memory>
#include "treenode.h"

class TreeModel : public QAbstractItemModel {
    Q_OBJECT

public:
    explicit TreeModel(QObject *parent = nullptr);
    ~TreeModel() override;

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    void setRootNode(std::unique_ptr<TreeNode> root);
    TreeNode *nodeFromIndex(const QModelIndex &index) const;
    void updateFullPaths();

private:
    void updateFullPathsRecursive(TreeNode *node, const QString &parentPath);
    std::unique_ptr<TreeNode> mRootNode;
};

#endif // TREEMODEL_H

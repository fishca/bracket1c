#ifndef TREENODE_H
#define TREENODE_H

#include <QString>
#include <QVariant>
#include <QList>
#include <memory>

class TreeNode {
public:
    explicit TreeNode(const QString &data = QString(), TreeNode *parent = nullptr);
    ~TreeNode();

    void appendChild(std::unique_ptr<TreeNode> child);
    TreeNode *child(int row) const;
    int childCount() const;
    int row() const;
    TreeNode *parent() const;

    void setData(const QString &data);
    QString data() const;
    void setFullPath(const QString &path);
    QString fullPath() const;

private:
    QString mData;
    QString mFullPath;
    TreeNode *mParent;
    QList<TreeNode*> mChildren;
};

#endif // TREENODE_H

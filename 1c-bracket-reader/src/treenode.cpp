#include "treenode.h"

TreeNode::TreeNode(const QString &data, TreeNode *parent)
    : mData(data)
    , mParent(parent)
{
}

TreeNode::~TreeNode()
{
    qDeleteAll(mChildren);
}

void TreeNode::appendChild(std::unique_ptr<TreeNode> child)
{
    if (child) {
        child->mParent = this;
        mChildren.append(child.release());
    }
}

TreeNode *TreeNode::child(int row) const
{
    if (row < 0 || row >= mChildren.size())
        return nullptr;
    return mChildren[row];
}

int TreeNode::childCount() const
{
    return mChildren.size();
}

int TreeNode::row() const
{
    if (mParent) {
        for (int i = 0; i < mParent->mChildren.size(); ++i) {
            if (mParent->mChildren[i] == this)
                return i;
        }
    }
    return 0;
}

TreeNode *TreeNode::parent() const
{
    return mParent;
}

void TreeNode::setData(const QString &data)
{
    mData = data;
}

QString TreeNode::data() const
{
    return mData;
}

void TreeNode::setFullPath(const QString &path)
{
    mFullPath = path;
}

QString TreeNode::fullPath() const
{
    return mFullPath;
}

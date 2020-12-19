#ifndef TREEITEM_H
#define TREEITEM_H

#include <QVariant>
#include <QVector> //动态数组的模板类


/*  树形类:
    使用基于指针的树结构意味着，当向视图传递模型索引时，我们可以记录索引中相应项目的地址。并在稍后使用QModelIndex::internalPointer()检索它。这使得编写模型更加容易，并确保引用同一项的所有模型索引具有相同的内部数据指针。
    这个类用于保存一个QVariants列表，包含列数据，以及关于列在结构中的位置信息

*/
class TreeItem
{
public:
    explicit TreeItem(const QVector<QVariant> &data, TreeItem *parentItem = nullptr);

    ~TreeItem();

    //向树添加数据
    void appendChild(TreeItem *child);

    TreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    TreeItem *parentItem();

private:
    QVector<TreeItem*> m_childItems;
    QVector<QVariant> m_itemData;
    TreeItem *m_parentItem;

};

#endif // TREEITEM_H

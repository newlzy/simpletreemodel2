#include "treeitem.h"


//记录项的父项和每一列相关联的数据
TreeItem::TreeItem(const QVector<QVariant> &data, TreeItem *parent):m_itemData(data),m_parentItem(parent)
{
}

TreeItem::~TreeItem()
{
    qDeleteAll(m_childItems); //删除
}

//在树形类末尾插入值
void TreeItem::appendChild(TreeItem *child)
{
    m_childItems.append(child);
}

//返回向量中位于索引位置的项
TreeItem *TreeItem::child(int row)
{
    if(row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row);
}

//返回vector中的项数
int TreeItem::childCount() const
{
    return m_childItems.count();
}

//返回vector中的项数
int TreeItem::columnCount() const
{
    return m_itemData.count();
}

//索引处的数据
QVariant TreeItem::data(int column) const
{
    if(column < 0 || column >= m_itemData.size())
        return QVariant();
    return m_itemData.at(column);
}

//第一个出现值的位置，行号
int TreeItem::row() const
{
    //indexOf返回向量中第一个出现值的索引位置
    if(m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<TreeItem*>(this));

}

//父项
TreeItem *TreeItem::parentItem()
{
    return m_parentItem;
}


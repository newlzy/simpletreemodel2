#include "treemodel.h"
#include "treeitem.h" //动态数组
#include <QDebug>

#include <QStringList>
#pragma execution_character_set("utf-8")

TreeModel::TreeModel(const QString &data, QObject *parent):QAbstractItemModel(parent)
{
    //创建数组
    rootItem = new TreeItem({tr("标题"),tr("总结")});
    //在'\n'出现的地方将字符串分割成子字符串，并返回这些字符串的列表
    //实现每一个节点的换行
    setupModelData(data.split('\n'),rootItem);
}

TreeModel::~TreeModel()
{
    delete rootItem;
}

//模型中索引处的数据
QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()) //模型索引有效
        return QVariant();

    if(role != Qt::DisplayRole) //文本形式呈现数据
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    //模型中索引处的数据
    return item->data(index.column());

}

//项标志
Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if(!index.isValid()) //索引无效
        return Qt::NoItemFlags; //没用任何属性设置
    //返回给定索引的项标志
    return QAbstractItemModel::flags(index);
}

//水平方向的数据
QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    //水平方向并且以文本格式呈现数据
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section); //索引处数据
    return QVariant();
}

//用数组中的项给行和列创建模型索引
QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row,column,parent)) //是父行列
        return QModelIndex();

    TreeItem *parentItem;
    if(!parent.isValid()) //索引无效
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    //位于row位置中的项
    TreeItem *childItem = parentItem->child(row);
    if(childItem)
        return createIndex(row,column,childItem); //为行和列创建模型索引
    return QModelIndex();
}

//创建父项模型索引
QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if(!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    //得到动态数组类对象指针
    TreeItem *parentItem = childItem->parentItem();
    if(parentItem == rootItem) //新数组和旧的相同
        return QModelIndex();

    //为最原始的数组创建模型索引，作为父项
    return createIndex(parentItem->row(),0,parentItem);
}

//数组中的项数，父
int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if(parent.column() > 0) //列大于0
        return 0;

    if(!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount(); //数组中的项数
}

//获取vector中的项数
int TreeModel::columnCount(const QModelIndex &parent) const
{
    //internalPointer模型使用该指针将索引与内部数据结构相关联。最后去获取vector中的项数
    if(parent.isValid()) //模型索引有效
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    return rootItem->columnCount(); //获取vector中的项数
}

//处理数据以何种方式显示
void TreeModel::setupModelData(const QStringList &lines, TreeItem *parent)
{
   QVector<TreeItem*> parents;
   QVector<int> indentations;
   parents << parent;
   indentations << 0;

   int number = 0;

   //遍历string列表。
   while(number < lines.count()){ //列表中的项数
       qDebug()<<"项数："<<lines.count();
       int position = 0; //空白计数
       //length字符数。at索引位置处的字符。不允许开头处有空白。
       while (position < lines[number].length()) {
           qDebug() <<"字符数："<< lines[number].length();
           if(lines[number].at(position) != ' ')
               break;
           position++;
       }

       //mid返回指定位置到末尾的字符。trimmed删除空白
       //把有空白的字符删除空白
       const QString lineData = lines[number].mid(position).trimmed();
       qDebug() << "position处的字符：" << lines[number].mid(position);
       if(!lineData.isEmpty()){ //是正常数据时
           //在\t出现的地方将字符串分割成子字符串。如果一个字段是空的，不要在结果中包含它
           const QStringList columnStrings = lineData.split(QLatin1Char('\t'),Qt::SkipEmptyParts);
           qDebug()<<columnStrings;

           QVector<QVariant> columnData; //分割好的字符串

           //reserve尝试为至少大小相同的元素分配内存。如果您提前知道vector的大小，则应该调用该函数以防止重新分配和内存碎片。
           columnData.reserve(columnStrings.count());
           for(const QString &columnString : columnStrings)
               columnData << columnString;

           //last对vector中最后一项的引用
           if(position > indentations.last()){

               //childCoun为tvector中的项数。当TreeItem类数组中有两项数据时进if
               if(parents.last()->childCount() > 0) {
                   //当前父节点的最后一个子节点现在是新父节点
                   //除非当前父节点没有子节点。
                   qDebug() << "TreeItem类：" << parents.last() << " --:" << parents.last()->childCount();
                   parents << parents.last()->child(parents.last()->childCount()-1);
                   indentations << position;
               }
           }else{
               while (position < indentations.last() && parents.count() > 0) {
                   //删除vector中的最后一项
                   parents.pop_back();
                   indentations.pop_back();
               }
           }

           //向当前父节点的子节点列表追加一个新项。
           parents.last()->appendChild(new TreeItem(columnData,parents.last()));

       }
        ++number;
   }

}




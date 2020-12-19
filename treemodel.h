#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>//模型抽象接口
class TreeItem;

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    TreeModel(const QString &data, QObject *parent = nullptr);
    ~TreeModel();

    //返回索引引用的项在给定角色下存储的数据
    QVariant data(const QModelIndex &index, int role) const override;

    //给定索引的项标志
    Qt::ItemFlags flags(const QModelIndex& index)const override;

    //对于水平标题，节号对应于列号。垂直标题，节号对应行号
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    //由给定的行、列和父索引指定的模型中项的索引
    QModelIndex index(int row,int column,const QModelIndex &parent = QModelIndex()) const override;

    //返回具有给定索引的模型项的父项。如果项没有父项，则返回一个无效的QModelIndex。
    QModelIndex parent(const QModelIndex &index) const override;

    //返回给定父目录下的行数。当父节点有效时，意味着rowCount返回父节点的子节点数。
    int rowCount(const QModelIndex &parent = QModelIndex())const override;

    //返回给定父节点的子节点的列数
    int columnCount(const QModelIndex &parent = QModelIndex())const override;

private:
    void setupModelData(const QStringList &lines,TreeItem *parent);

    TreeItem *rootItem;

};

#endif // TREEMODEL_H

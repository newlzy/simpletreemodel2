#include <QApplication>
#include <QFile>  //读取文件
#include <QTreeView> //树视图
#include "treemodel.h"
#pragma execution_character_set("utf-8")

int main(int argc, char* argv[]){

    //资源在启动时初始化
    Q_INIT_RESOURCE(simpletreemodel);

    QApplication app(argc,argv);

    QFile file(":/default.txt");
    file.open(QIODevice::ReadOnly); //可读
    //readAll读取所有数据，并以字节数组形式返回。
    TreeModel model(file.readAll());
    file.close(); //关闭读取

    QTreeView view;
    view.setModel(&model);
    view.setWindowTitle(QObject::tr("简单的树模型"));
    view.show();

    return app.exec();

}

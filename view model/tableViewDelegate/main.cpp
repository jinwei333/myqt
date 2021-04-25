#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#if 0
    QStandardItemModel model(4, 2);
    QTableView tableView;
    tableView.setModel(&model);

    SpinBoxDelegate delegate(0, 100, nullptr);
    tableView.setItemDelegate(&delegate);

    for (int row = 0; row < 4; ++row) {
        for (int column = 0; column < 2; ++column) {
            QModelIndex index = model.index(row, column, QModelIndex());
            model.setData(index, QVariant((row + 1) * (column + 1)));
        }
    }

    tableView.setWindowTitle(QObject::tr("Spin Box Delegate"));
    tableView.show();
#else
    MainWindow w;
    w.show();

#endif

    return a.exec();
}

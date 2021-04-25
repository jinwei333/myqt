#include "widget.h"
#include "ui_widget.h"
#include <QCompleter>
#include <QFileSystemModel>
#include <QDirModel>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // auto completions from a simple word list
    QStringList wordList;
    wordList << "alpha" << "omega" << "omicron" << "zeta";

    QCompleter *completer = new QCompleter(wordList, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->lineEdit->setCompleter(completer);

    QCompleter *fileCompleter = new QCompleter(this);
    fileCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    QFileSystemModel *fileModel = new QFileSystemModel(fileCompleter);
    fileModel->setRootPath(QDir::currentPath());
    fileCompleter->setModel(fileModel);
    ui->fileLineEdit->setCompleter(fileCompleter);

    QCompleter *dirCompleter = new QCompleter(this);
    dirCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    dirCompleter->setModel(new QDirModel(dirCompleter));
    ui->dirLineEdit->setCompleter(dirCompleter);
}

Widget::~Widget()
{
    delete ui;
}


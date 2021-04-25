#include "widget.h"
#include "ui_widget.h"
#include <QStringListModel>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    setWindowTitle(tr("Simple Widget Mapper"));
    setFormModel();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::setFormModel()
{
    m_Model = new QStandardItemModel(3, 3, this);

    QStringList names;
    names << "Alice" << "Bob" << "Carol" << "Clam";

    QVector<int> ages;
    ages << 18 << 28 << 38 << 48;

    QStringList items;
    items << tr("女") << tr("男") << tr("人妖");
//  we need a way to relate their input to the values stored in the model.
    QStringListModel *sexModel = new QStringListModel(items, this);
    ui->sexComboBox->setModel(sexModel);

    QStringList sexs;
    sexs << "0" << "1" << "2" << "-1";

    for(int row = 0; row < 4; ++row) {
        m_Model->setItem(row, 0, new QStandardItem(names[row]));
        m_Model->setItem(row, 1, new QStandardItem(QString::number(ages[row])));
        m_Model->setItem(row, 2, new QStandardItem(sexs[row]));
    }

    m_Mapper = new QDataWidgetMapper(this);
    m_Mapper->setModel(m_Model);
    m_Mapper->addMapping(ui->nameLineEdit, 0);
    m_Mapper->addMapping(ui->ageSpinBox, 1);

    /// 第三个参数请参考 Q_PROPERTY
    m_Mapper->addMapping(ui->sexComboBox, 2, "currentIndex");

    connect(ui->prevBtn, &QPushButton::clicked, m_Mapper, &QDataWidgetMapper::toPrevious);
    connect(ui->nextBtn, &QPushButton::clicked, m_Mapper, &QDataWidgetMapper::toNext);
    connect(m_Mapper, &QDataWidgetMapper::currentIndexChanged, this, &Widget::updateButtons);
            m_Mapper->toFirst();
}

void Widget::updateButtons(int row)
{
    ui->prevBtn->setEnabled(row > 0);
    ui->nextBtn->setEnabled(row < (m_Model->rowCount()-1));
}


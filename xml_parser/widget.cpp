#include "widget.h"
#include "ui_widget.h"
#include <QXmlStreamReader>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

/*
    1. XML 指可扩展标记语言（EXtensible Markup Language）。
    2. XML 文档必须包含根元素。该元素是所有其他元素的父元素。
*/
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}



void Widget::xmlFileParse(QString fileName)
{
    if(fileName.isEmpty())
        return;

    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly) != true) {
        QMessageBox::critical(this, "Open failed", tr("File %1 open failed!"));
        return;
    }

    QXmlStreamReader reader(&file);

    while (!reader.atEnd()) {
        QXmlStreamReader::TokenType tokenType = reader.readNext();

        switch (tokenType) {
        case QXmlStreamReader::StartDocument:   // 开始文档
            setEditorFont(Qt::black);
            outputDebug("开始文档");
            outputDebug("Version: " + reader.documentVersion().toString());
            outputDebug("Encode: " + reader.documentEncoding().toString());
            outputDebug("Standalone :" + QString(reader.isStandaloneDocument()?"true":"false"));
            break;
        case QXmlStreamReader::Comment: // 注释
            setEditorFont(Qt::gray);
            outputDebug("\r\n注释");
            outputDebug(reader.text().toString());
            break;
        case QXmlStreamReader::ProcessingInstruction:   // 处理指令
            setEditorFont(Qt::green);
            outputDebug("\r\n处理指令");
            outputDebug(reader.processingInstructionData().toString());
            break;
        case QXmlStreamReader::DTD:
        {
            setEditorFont(Qt::red);
            outputDebug("\r\nDTD");
//            QString dtd = reader.text().toString();
            QXmlStreamNotationDeclarations notationDeclarations = reader.notationDeclarations();

            outputDebug("======notationDeclaration");
            foreach (auto &notationDeclaration, notationDeclarations) {
                outputDebug(notationDeclaration.name().toString());
            }
            outputDebug("======entityDeclarations");
            QXmlStreamEntityDeclarations entityDeclarations = reader.entityDeclarations();
            foreach (auto &entityDeclaration, entityDeclarations) {
                outputDebug(entityDeclaration.name().toString());
            }
            outputDebug("dtdName" + reader.dtdName().toString());
            outputDebug("Public id: " + reader.dtdPublicId().toString());
            outputDebug("System id: " + reader.dtdSystemId().toString());
            break;
        }
        case QXmlStreamReader::StartElement:
        {
            setEditorFont(Qt::blue);
//            outputDebug("StartElement");
            outputDebug("namespaceUri: " + reader.namespaceUri().toString());
            outputDebug(reader.name().toString());

            QXmlStreamAttributes attributes = reader.attributes();

            foreach (auto &attribute, attributes) {
                outputDebug(QString("Attribute: %1, Value: %2").arg(attribute.name().toString()).arg(attribute.value().toString()));
            }
            break;
        }
        case QXmlStreamReader::EndDocument:
            setEditorFont(Qt::black);
            outputDebug("\r\n结束文档");
            break;
        default:
            break;
        }

    }
    if(reader.hasError()) {
        outputDebug(QString("出错，错误信息：%1 行号：%2 列号：%3 字符移位：%4").arg(
                        reader.errorString()).arg(
                        reader.lineNumber()).arg(
                        reader.columnNumber()).arg(
                        reader.characterOffset()));
    }
    file.close();
}

void Widget::outputDebug(const QString &text)
{
    if(text.isEmpty())
        return;
    ui->textEdit->append(text);
}

void Widget::setEditorFont(Qt::GlobalColor color)
{
    ui->textEdit->setTextColor(color);
}

void Widget::openXmlFile()
{
    QString fileName =  QFileDialog::getOpenFileName(this, tr("Open xml file"),
                                                     QDir::currentPath(), tr("Xml (*.xml)"));

    if(fileName.isEmpty())
        return;

    ui->fileLineEdit->setText(fileName);
    xmlFileParse(fileName);
}


void Widget::on_parserBtn_clicked()
{
    openXmlFile();
}

#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QXmlStreamReader>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void xmlFileParse(QString fileName);
    void outputDebug(const QString &text);

    void setEditorFont(Qt::GlobalColor color);
private slots:
    void on_parserBtn_clicked();

private:
    void openXmlFile();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H

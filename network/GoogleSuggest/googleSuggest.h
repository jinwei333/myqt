#ifndef GOOGLESUGGEST_H
#define GOOGLESUGGEST_H

#include <QObject>
#include <QLineEdit>
#include <QTreeWidget>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QListWidget>
#include <QNetworkReply>

class GoogleSuggest : public QObject
{
    Q_OBJECT
public:
    explicit GoogleSuggest(QLineEdit *parent = nullptr);
    ~GoogleSuggest();

    void preventSuggest();


private slots:
    void doneCompletion(const QModelIndex &index);

    void startSuggest();

    void processNetworkData(QNetworkReply *reply);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    void showCompletion(const QStringList &choices);

    QLineEdit *m_editor;
    QListWidget *m_popup;
    QTimer *m_timer;  // 用户输入500ms后弹出搜索值
    QNetworkAccessManager m_networkManager;
};

#endif // GOOGLESUGGEST_H

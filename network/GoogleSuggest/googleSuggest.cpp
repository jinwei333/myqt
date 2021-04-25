#include "googleSuggest.h"
#include <QXmlStreamReader>
#include <QStandardItem>
#include <QListWidgetItem>
#include <QEvent>
#include <QKeyEvent>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

GoogleSuggest::GoogleSuggest(QLineEdit *parent) : QObject(parent), m_editor(parent)
{
    m_popup = new QListWidget;
    m_popup->setWindowFlags(Qt::Popup);

    // 接受键盘焦点的方式
    m_popup->setFocusPolicy(Qt::NoFocus);

    m_popup->setFocusProxy(parent);

    // 即使没按下鼠标按钮，小部件也会接收鼠标移动事件。
    m_popup->setMouseTracking(true);

    m_popup->setFrameStyle(QFrame::Box | QFrame::Plain);

    m_popup->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_popup->installEventFilter(this);

    m_popup->setStyleSheet("QListWidget{border-top: none;}");

    connect(m_popup, &QListView::clicked, this, &GoogleSuggest::doneCompletion);

    m_timer = new QTimer(this);
    m_timer->setSingleShot(true);
    m_timer->setInterval(500);

    connect(m_timer, &QTimer::timeout, this, &GoogleSuggest::startSuggest);
    connect(m_editor, &QLineEdit::textEdited, [=](){
        m_timer->start();
    });
    connect(&m_networkManager, &QNetworkAccessManager::finished, this,
            &GoogleSuggest::processNetworkData);
}

GoogleSuggest::~GoogleSuggest()
{
    delete m_popup;
}

void GoogleSuggest::preventSuggest()
{
    m_timer->stop();
}

void GoogleSuggest::doneCompletion(const QModelIndex &index)
{
    Q_UNUSED(index);

    m_timer->stop();
    m_popup->hide();
    m_editor->setFocus();

    QListWidgetItem *item = m_popup->currentItem();
    if(item) {
        m_editor->setText(item->text());
        QMetaObject::invokeMethod(m_editor, "returnPressed");
    }
}

#define GSUGGEST_URL "https://www.baidu.com/sugrec?prod=pc&wd=%1"

void GoogleSuggest::startSuggest()
{
    QString text = m_editor->text();

    if(text.isEmpty()) {
        return;
    }
    QUrl url(QString(GSUGGEST_URL).arg(text));
    m_networkManager.get(QNetworkRequest(url));
}

void GoogleSuggest::processNetworkData(QNetworkReply *reply)
{
    qDebug() << reply->error();

    if(reply->error() == QNetworkReply::NoError) {
        QStringList choices;

        QByteArray response(reply->readAll());

        QJsonDocument doc(QJsonDocument::fromJson(response));
        QJsonObject jsonObj = doc.object();
        if(jsonObj.contains("g")) {
            QJsonArray arr = jsonObj.value("g").toArray();
            for(const auto &item : arr) {
                choices << item.toObject().value("q").toString();
            }

        }
#if 0
        QXmlStreamReader xmlReader(response);

        while(!xmlReader.atEnd()) {
            QXmlStreamReader::TokenType type = xmlReader.readNext();

            if(type == QXmlStreamReader::StartElement) {
                if(xmlReader.name() == "suggestion") {
                    choices << xmlReader.attributes().value("Version").toString();
                }
            }
        }
#endif
        showCompletion(choices);
    }

    reply->deleteLater();
}

bool GoogleSuggest::eventFilter(QObject *watched, QEvent *event)
{
    if(watched != m_popup)
        return false;

    if(event->type() == QEvent::MouseButtonPress) {
        m_popup->hide();
        m_editor->setFocus();
        return true;
    }

    if(event->type() == QEvent::KeyPress)
    {
        int key = static_cast<QKeyEvent*>(event)->key();

        switch (key) {
        case Qt::Key_Enter:
        case Qt::Key_Return:
            doneCompletion(QModelIndex());
            return true;

        case Qt::Key_Escape:
            m_editor->setFocus();
            m_popup->hide();
            return true;

        case Qt::Key_Up:
        case Qt::Key_Down:
        case Qt::Key_Home:
        case Qt::Key_End:
        case Qt::Key_PageUp:
        case Qt::Key_PageDown:
            break;

        default:
            m_editor->setFocus();
            m_editor->event(event);
            m_popup->hide();
            break;
        }
    }

    return false;
}

void GoogleSuggest::showCompletion(const QStringList &choices)
{
    if(choices.isEmpty())
        return;

    const QPalette &palette = m_editor->palette();
    QColor color = palette.color(QPalette::Active, QPalette::WindowText);

    // 当小部件在被其他小部件替换之前被隐藏时，服务器可能会删除屏幕上的区域。
    m_popup->setUpdatesEnabled(false);

    m_popup->clear();
    for(int i = 0; i < choices.count(); i++) {
        QListWidgetItem *item = new QListWidgetItem(m_popup);
        item->setText(choices.at(i));
        item->setTextColor(color);
    }
    m_popup->setCurrentItem(m_popup->item(0));
    m_popup->setUpdatesEnabled(true);
    m_popup->move(m_editor->mapToGlobal(QPoint(0, m_editor->height())));
    m_popup->setFocus();
    m_popup->show();
}

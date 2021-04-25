#include "searchBox.h"
#include <QDesktopServices>


#define GSEARCH_URL "https://www.baidu.com/s?wd=%1"

SearchBox::SearchBox(QWidget *parent)  : QLineEdit(parent)
{
    m_completer = new GoogleSuggest(this);

    connect(this, &SearchBox::returnPressed, this, &SearchBox::doSearch);
    setFocus();
}

void SearchBox::doSearch()
{
    m_completer->preventSuggest();
    QString url = QString(GSEARCH_URL).arg(text());
    QDesktopServices::openUrl(url);
}

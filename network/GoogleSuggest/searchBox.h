#ifndef SEARCHBOX_H
#define SEARCHBOX_H

#include <QObject>
#include <QLineEdit>
#include "googleSuggest.h"

class SearchBox : public QLineEdit
{
    Q_OBJECT
public:
    SearchBox(QWidget *parent = Q_NULLPTR);

private slots:
    void doSearch();

private:
    GoogleSuggest *m_completer;
};

#endif // SEARCHBOX_H

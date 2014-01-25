
#pragma once

#include <QObject>

class QWidget;


class DropFix : public QObject
{
public:
    DropFix(QWidget * parent);
    virtual ~DropFix();

protected:
    virtual bool eventFilter(QObject * obj, QEvent * event);
};

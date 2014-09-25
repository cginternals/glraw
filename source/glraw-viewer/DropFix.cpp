
#include "DropFix.h"

#include <QEvent>
#include <QWidget>

DropFix::DropFix(QWidget * parent)
: QObject(parent)
{
}

DropFix::~DropFix()
{
}

bool DropFix::eventFilter(QObject * obj, QEvent * event)
{
    switch (event->type())
    {
    case QEvent::DragEnter:
    case QEvent::DragLeave:
    case QEvent::DragMove:
    case QEvent::Drop:
        parent()->event(event);
        return true;
    default:
        return QObject::eventFilter(obj, event);
    }
}

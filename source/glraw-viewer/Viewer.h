
#pragma once

#include <QMainWindow>

class Ui_Viewer;

class QLabel;
class QSurfaceFormat;
class QShortcut;


class Viewer : public QMainWindow
{
    Q_OBJECT

public:
    Viewer(
        QWidget * parent = nullptr
    ,   Qt::WindowFlags flags = NULL);

    virtual ~Viewer();

protected:
	const QScopedPointer<Ui_Viewer> m_ui;
};

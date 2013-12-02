
#include <sstream>
#include <iomanip>

#include <QOpenGLContext>
#include <QShortcut>
#include <QSettings>

#include "Application.h"
#include "Viewer.h"

#include "ui_Viewer.h"


namespace 
{
    const QString SETTINGS_GEOMETRY ("Geometry");
    const QString SETTINGS_STATE    ("State");
}

Viewer::Viewer(
    QWidget * parent,
    Qt::WindowFlags flags)

: QMainWindow(parent, flags)
, m_ui(new Ui_Viewer)
{
    m_ui->setupUi(this);
    setWindowTitle(Application::title());

    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings s;

    restoreGeometry(s.value(SETTINGS_GEOMETRY).toByteArray());
    restoreState(s.value(SETTINGS_STATE).toByteArray());
};

Viewer::~Viewer()
{
    QSettings s;
    s.setValue(SETTINGS_GEOMETRY, saveGeometry());
    s.setValue(SETTINGS_STATE, saveState());
}

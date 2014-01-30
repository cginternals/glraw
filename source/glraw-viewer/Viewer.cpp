
#include "Viewer.h"

#include <sstream>
#include <iomanip>

#include <QOpenGLContext>
#include <QShortcut>
#include <QString>
#include <QStringList>
#include <QSettings>
#include <QDropEvent>
#include <QMimeData>
#include <QFileInfo>

#include "Application.h"

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

    setAcceptDrops(true);
}

Viewer::~Viewer()
{
    QSettings s;
    s.setValue(SETTINGS_GEOMETRY, saveGeometry());
    s.setValue(SETTINGS_STATE, saveState());
}

void Viewer::dragEnterEvent(QDragEnterEvent * event)
{
    if (event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
}

void Viewer::dragMoveEvent(QDragMoveEvent * event)
{
    if (event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
}

void Viewer::dropEvent(QDropEvent * event)
{
    if (event->mimeData()->hasFormat("text/uri-list"))
    {
        const QStringList fileNames = event->mimeData()->text().split("\n");
        QString fileName(fileNames.first().trimmed());

        fileName.remove("file:///"); // window fix
        fileName.remove("file://");

        emit fileDropped(fileName);

        event->acceptProposedAction();
    }
}

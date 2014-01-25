
#include <QSurfaceFormat>
#include <QWidget>

#include "Application.h"

#include "DropFix.h"
#include "Viewer.h"
#include "Canvas.h"


int main(int argc, char* argv[])
{
    Application app(argc, argv);

    QScopedPointer<Viewer> viewer(new Viewer());

    QSurfaceFormat format;
    format.setVersion(3, 2);
    format.setProfile(QSurfaceFormat::CoreProfile);

    Canvas * canvas = new Canvas(format);

    DropFix * dropfix = new DropFix(viewer.data());
    QWidget * widget = QWidget::createWindowContainer(canvas);

    viewer->setAcceptDrops(true);
    widget->setAcceptDrops(true);

    canvas->installEventFilter(dropfix);

    widget->setMinimumSize(1, 1);
    widget->setAutoFillBackground(false); // Important for overdraw, not occluding the scene.

    QObject::connect(viewer.data(), &Viewer::fileDropped, canvas, &Canvas::loadFile);

    viewer->setCentralWidget(widget);
    viewer->show();

    return app.exec();
}

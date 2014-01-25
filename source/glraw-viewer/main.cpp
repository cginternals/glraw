
#include <QSurfaceFormat>
#include <QWidget>

#include "Application.h"

#include "Viewer.h"
#include "Canvas.h"


int main(int argc, char* argv[])
{
    Application app(argc, argv);

    QScopedPointer<Viewer> viewer(new Viewer());

    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);

    Canvas * canvas = new Canvas(format);
    QWidget * widget = QWidget::createWindowContainer(canvas);

    widget->setAcceptDrops(true);

    widget->setMinimumSize(1, 1);
    widget->setAutoFillBackground(false); // Important for overdraw, not occluding the scene.
    widget->setFocusPolicy(Qt::TabFocus);

    QObject::connect(viewer.data(), &Viewer::fileDropped, canvas, &Canvas::loadFile);

    viewer->setCentralWidget(widget);
    viewer->show();

    return app.exec();
}

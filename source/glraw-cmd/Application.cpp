
#include "Application.h"

#include <QFileInfo>
#include "glraw-cmd-version.h"

Application::Application(int & argc, char ** argv)
:   QApplication(argc, argv)
{
    QCoreApplication::setApplicationName(GLRAW_PROJECT_NAME);
    QCoreApplication::setApplicationVersion(GLRAW_VERSION);

    QCoreApplication::setOrganizationName(GLRAW_AUTHOR_ORGANIZATION);
    QCoreApplication::setOrganizationDomain(GLRAW_AUTHOR_DOMAIN);
}

Application::~Application()
{
}

const QString Application::title() const
{
    return QString("%2 %3")
        .arg(QCoreApplication::applicationName())
        .arg(QCoreApplication::applicationVersion());
}

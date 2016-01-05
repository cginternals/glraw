
#include "Application.h"

#include <glraw/glraw-version.h>

Application::Application(
    int & argc
,   char ** argv)
: QApplication(argc, argv)
{
	QApplication::setApplicationName(GLRAW_PROJECT_NAME);
	QApplication::setApplicationVersion(GLRAW_VERSION);

	QApplication::setOrganizationName(GLRAW_AUTHOR_ORGANIZATION);
	QApplication::setOrganizationDomain(GLRAW_AUTHOR_DOMAIN);
}

Application::~Application()
{
}

const QString Application::title()
{
    return QString("%1-viewer %2")
        .arg(QApplication::applicationName())
        .arg(QApplication::applicationVersion());
}


#pragma once

#include <QCoreApplication>

class Application : public QCoreApplication
{
public:
    Application(int & argc, char ** argv);
    virtual ~Application();

    virtual const QString title() const;

protected:

};

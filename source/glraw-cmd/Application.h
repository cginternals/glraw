
#pragma once

#include <QApplication>

class Application : public QApplication
{
public:
    Application(int & argc, char ** argv);
    virtual ~Application();

    virtual const QString title() const;

protected:

};

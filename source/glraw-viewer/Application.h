
#pragma once

#include <QApplication>

// Application initializes Application strings from CMake generated Meta.
// It furhter installs a Message Handler for extended logging capabilities.

class Application : public QApplication
{
public:
    Application(int & argc, char ** argv);
    virtual ~Application();

    static const QString title();
};
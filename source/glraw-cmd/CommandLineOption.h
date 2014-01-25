
#pragma once

#include "Builder.h"

struct CommandLineOption
{
    // at least visual studio 2013 requires this constructor to 
    // correctly resolve initializer lists at run-time...
    CommandLineOption(
        const QStringList & names
    ,   QString description
    ,   QString valueName
    ,   Builder::ConfigureMethod configureMethod)
    : names(names)
    , description(description)
    , valueName(valueName)
    , configureMethod(configureMethod)
    {
    }

    QStringList names;
    QString description;
    QString valueName;
    Builder::ConfigureMethod configureMethod;
};

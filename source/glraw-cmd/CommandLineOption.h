
#pragma once

#include "Builder.h"

struct CommandLineOption
{
    QStringList names;
    QString description;
    QString valueName;
    Builder::ConfigureMethod configureMethod;
};

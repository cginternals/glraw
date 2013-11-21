
#pragma once

#include "Builder.h"

struct CommandLineOption
{
    QStringList names;
    QString description;
    QString valueName;
    bool (Builder::*configureMethod)(const QString &);
};

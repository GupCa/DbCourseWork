#pragma once

#include <QString>
#include <QObject>

/* Класс произвольных исключений. */
class CExceptions
{
public:
    virtual QString message() const = 0;
    virtual int code() const = 0;
};

/* Класс исключений отказа пользователя от авторизации. */
class CUserFail : public CExceptions
{
public:
    QString message() const { return QObject::tr("Пользователь отказался от авторизации."); }
    int code() const { return 0; }
};


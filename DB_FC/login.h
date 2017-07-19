#pragma once

#include <QDialog>

class QLineEdit;

/* Класс окна авторизации пользователя. */
class CLogin : public QDialog
{
public:
    CLogin(QWidget *pOwner = NULL);

    QString userName() const;
    QString password() const;

protected:
    void showEvent(QShowEvent * pEvent);

private:
    typedef QDialog inherited;

    QLineEdit *m_ptxtUserName;
    QLineEdit *m_ptxtPassword;
};


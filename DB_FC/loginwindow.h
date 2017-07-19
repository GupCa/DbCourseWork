#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QPicture>
#include <QImage>
#include <QPainter>
#include <QDialog>

namespace Ui {
class loginWindow;
}

class loginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit loginWindow(QWidget *parent = 0);
    explicit loginWindow(QWidget *parent = 0, QString El = "");
    ~loginWindow();

    QString getLogin();
    QString getPw();

private:
    Ui::loginWindow *ui;
};

#endif // LOGINWINDOW_H

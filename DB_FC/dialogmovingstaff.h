#ifndef DIALOGMOVINGSTAFF_H
#define DIALOGMOVINGSTAFF_H

#include <QDialog>
#include <QWidget>
#include <QtSql>

namespace Ui {
class Dialogmovingstaff;
}

class Dialogmovingstaff : public QDialog
{
    Q_OBJECT

public:
    explicit Dialogmovingstaff(QWidget *parent = 0, QSqlDatabase db1 = QSqlDatabase(), int id_worker1 = 0);
    ~Dialogmovingstaff();

signals:
    void staff_moved();

private slots:
    void refresh_spin_slider();
    void on_pushButton_clicked();

private:
    Ui::Dialogmovingstaff *ui;
    QSqlDatabase db;
    int id_worker;
};

#endif // DIALOGMOVINGSTAFF_H

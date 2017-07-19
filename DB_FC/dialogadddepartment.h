#ifndef DIALOGADDDEPARTMENT_H
#define DIALOGADDDEPARTMENT_H

#include <QWidget>
#include <QDialog>
#include <QtSql>
#include <QCompleter>

namespace Ui {
class Dialogadddepartment;
}

class Dialogadddepartment : public QDialog
{
    Q_OBJECT

public:
    explicit Dialogadddepartment(QWidget *parent = 0, QSqlDatabase db1 = QSqlDatabase());
    ~Dialogadddepartment();

private slots:
    void DepartmentInsert();
    void on_pushButton_clicked();

private:
    Ui::Dialogadddepartment *ui;
    QSqlDatabase db;
};

#endif // DIALOGADDDEPARTMENT_H

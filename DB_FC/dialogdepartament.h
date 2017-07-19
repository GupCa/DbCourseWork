#ifndef DIALOGDEPARTAMENT_H
#define DIALOGDEPARTAMENT_H

#include <QWidget>
#include <QDialog>
#include <QtSql>
#include <QCompleter>

namespace Ui {
class DialogDepartament;
}

class DialogDepartament : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDepartament(QWidget *parent = 0, QSqlDatabase db1 = QSqlDatabase());
    ~DialogDepartament();

private slots:
    void DepartmentInsert();
    void setmin(int);

    void on_pushButton_clicked();
signals:
    void department_inserted();

private:
    Ui::DialogDepartament *ui;
    QSqlDatabase db;
};

#endif // DIALOGDEPARTAMENT_H

#ifndef ADDABSENCE_H
#define ADDABSENCE_H

#include <QDialog>
#include <QtSql>


namespace Ui {
class AddAbsence;
}

class AddAbsence : public QDialog
{
    Q_OBJECT

public:
    explicit AddAbsence(QWidget *parent = 0, QSqlDatabase db1 = QSqlDatabase());
    ~AddAbsence();

signals:
    void absence_inserted();

private slots:
    void AbsenceDialog();
    void on_ok_button_clicked();
    void setmin(QDate);

private:
    Ui::AddAbsence *ui;
    QSqlDatabase db;
};

#endif // ADDABSENCE_H

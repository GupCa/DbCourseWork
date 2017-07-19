#ifndef ADDPHYSICAL_H
#define ADDPHYSICAL_H

#include <QWidget>
#include <QDialog>
#include <QtSql>
#include <QCompleter>

namespace Ui {
class AddPhysical;
}

class AddPhysical : public QDialog
{
    Q_OBJECT

public:
    explicit AddPhysical(QWidget *parent = 0, QSqlDatabase db1 = QSqlDatabase());
    ~AddPhysical();
signals:
    void individual_inserted();
private slots:
    void IndividualInsert();

    void on_buttonBox_accepted();

private:
    Ui::AddPhysical *ui;
    QSqlDatabase db;
};

#endif // ADDPHYSICAL_H

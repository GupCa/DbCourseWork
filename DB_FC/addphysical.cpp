#include "addphysical.h"
#include "ui_addphysical.h"
#include <QDialog>
#include <QLineEdit>
#include <string>

AddPhysical::AddPhysical(QWidget *parent, QSqlDatabase db1) :
    QDialog(parent),
    ui(new Ui::AddPhysical)
{
    db = db1;
    ui->setupUi(this);
}

AddPhysical::~AddPhysical()
{
    delete ui;
}

void AddPhysical::IndividualInsert()
{
    QString query_function;
        QString name, surname, pathronymic;
        QDate date;
        int phone;
        name = ui->name_edit->text();
        surname = ui->surname_edit->text();
        pathronymic = ui->patronymic_edit->text();
        phone = ui->phone_edit->text().toInt();
        date = ui->date_edit->date();

        query_function = QString("SELECT schema1.add_individual(:name,:surname,:pathronymic,:date,:phone)");
                //.arg( name, surname, pathronymic,  date, phone);
        QSqlQuery* query = new QSqlQuery(db);
        query->prepare(query_function);
        query->bindValue(":name", name);
        query->bindValue(":surname", surname);
        query->bindValue(":pathronymic", pathronymic);
        query->bindValue(":date", date);
        query->bindValue(":phone", phone);
        query->exec();
        //query->next();

//        query_function = QString("SELECT ""schema1.add_individual""('gnklfd','sdklf','dsf','2001-01-01','32423');");
//        QSqlQuery*  query = new QSqlQuery;
//        query->exec(query_function);
//        query->next();

        qDebug() << query_function;
        qDebug() << db.lastError();

        emit individual_inserted();
}

void AddPhysical::on_buttonBox_accepted()
{
    IndividualInsert();
}

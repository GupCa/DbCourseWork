#include "dialogadddepartment.h"
#include "ui_dialogadddepartment.h"
#include <QDialog>
#include <QLineEdit>
#include <string>

Dialogadddepartment::Dialogadddepartment(QWidget *parent, QSqlDatabase db1) :
    QDialog(parent),
    ui(new Ui::Dialogadddepartment)
{
    db = db1;
    ui->setupUi(this);
    QRegExp rx ("[А-ЯІЄЇҐа-яієїґ]{3,20}");
    ui->lineEdit->setValidator (new QRegExpValidator (rx, this));
}

Dialogadddepartment::~Dialogadddepartment()
{
    delete ui;
}
void Dialogadddepartment::DepartmentInsert()
{
    QRegExp rx ("[А-ЯІЄЇҐа-яієїґ]{3,20}");
    if (rx.exactMatch(ui->lineEdit->text()) ) {
        QString query_function;
        QString name;
        name = ui->lineEdit->text();
        query_function = QString("insert INTO schema1.department(name, id_counterparty) VALUES (:name,currval('schema1.counterparty_id_counterparty_seq'));");
        QSqlQuery* query = new QSqlQuery(db);
        query->prepare(query_function);
        query->bindValue(":name", name);
        query->exec();

        if (query->lastError().isValid()) {
            ui->error_label->setStyleSheet("QLabel { color : red; }");
            std::string s = query->lastError().text().toUtf8().constData();;
            s = s.substr(0,s.find("(P0001)"));
            ui->error_label->setWordWrap(true);
            ui->error_label->setText(QString::fromUtf8(s.c_str()));
        }
        else {
            Dialogadddepartment::close();
        }
    }
    else {
        ui->error_label->setStyleSheet("QLabel { color : red; }");
        ui->error_label->setText("Введена назва некоректна");
    }
}

void Dialogadddepartment::on_pushButton_clicked()
{
    DepartmentInsert();
}

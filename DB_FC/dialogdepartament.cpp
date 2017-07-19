#include "dialogdepartament.h"
#include "ui_dialogdepartament.h"
#include <QDialog>
#include <QLineEdit>
#include <string>

DialogDepartament::DialogDepartament(QWidget *parent, QSqlDatabase db1) :
    QDialog(parent),
    ui(new Ui::DialogDepartament)
{
    db = db1;
    ui->setupUi(this);
    QRegExp rx ("[А-ЯІЄЇҐа-яієїґ]{3,20}");
    ui->position_lineEdit->setValidator (new QRegExpValidator (rx, this));
    QString query_s;
    query_s = QString("select schema1.department.id_department, schema1.department.name FROM schema1.department;");
    QSqlQuery*  query = new QSqlQuery(db);
    query->exec(query_s);
    while (query->next())
    {
        ui->department_comboBox->addItem(query->value(1).toString(), query->value(0).toInt());
    }
    QObject::connect(ui->minsal_spinBox,SIGNAL(valueChanged(int)),this,SLOT(setmin(int)));
}

void DialogDepartament::setmin(int min)
{
    ui->maxsal_spinBox->setMinimum(min);
}


DialogDepartament::~DialogDepartament()
{
    delete ui;
}

void DialogDepartament::DepartmentInsert()
{
    QRegExp rx ("[А-ЯІЄЇҐа-яієїґ]{3,20}");
    if (rx.exactMatch(ui->position_lineEdit->text()) ) {
        int id_department = ui->department_comboBox->currentData().toInt();
        QString name_position = ui->position_lineEdit->text();
        int total_member = ui->totalm_spinBox->value();
        int min_s = ui->minsal_spinBox->value();
        int max_s = ui->maxsal_spinBox->value();
        QString query_function;

        query_function = QString("SELECT schema1.add_position(:id_department, :name_position, :total_member, :min_s, :max_s) ");
        QSqlQuery* query = new QSqlQuery(db);
        query->prepare(query_function);
        query->bindValue(":id_department", id_department);
        query->bindValue(":name_position", name_position);
        query->bindValue(":total_member", total_member);
        query->bindValue(":min_s", min_s);
        query->bindValue(":max_s", max_s);
        query->exec();

        qDebug() << query_function;
        qDebug() << query->lastError().text();

        if (query->lastError().isValid()) {
            ui->error_label->setStyleSheet("QLabel { color : red; }");
            std::string s = query->lastError().text().toUtf8().constData();;
            s = s.substr(0,s.find("(P0001)"));
            ui->error_label->setWordWrap(true);
            ui->error_label->setText(QString::fromUtf8(s.c_str()));
        }
        else {
            emit department_inserted();
            emit accepted();
            DialogDepartament::close();
        }
    }
    else {
        ui->error_label->setStyleSheet("QLabel { color : red; }");
        ui->error_label->setText("Введена назва некоректна");
    }
}

void DialogDepartament::on_pushButton_clicked()
{
    DepartmentInsert();
}

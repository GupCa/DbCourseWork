#include "addabsence.h"
#include "ui_addabsence.h"
#include <QDialog>
#include <string>
#include <QItemSelectionModel>
#include <QSqlQueryModel>

AddAbsence::AddAbsence(QWidget *parent, QSqlDatabase db1) :
    QDialog(parent),
    ui(new Ui::AddAbsence)
{
    db = db1;
    ui->setupUi(this);

    QSqlQueryModel *model = new QSqlQueryModel;
        model->setQuery("select * from schema1.worker_info;" );
        model->setHeaderData(0, Qt::Horizontal, "ім'я");
        model->setHeaderData(1, Qt::Horizontal, "призвіще");
        model->setHeaderData(2, Qt::Horizontal, "посада");

        ui->tableView->setModel(model);
        ui->tableView->setColumnHidden(3, true);
    QDate d1(2017, 1, 1);
    ui->dateEdit->setDate(d1);
    ui->dateEdit_2->setDate(QDate::currentDate().addDays(-1));

    QString query_s;
    query_s = QString("SELECT schema1.absence_type.id_absence_type, schema1.absence_type.type_desctription from schema1.absence_type;");
    QSqlQuery*  query = new QSqlQuery(db);
    query->exec(query_s);
    while (query->next())
    {
        ui->comboBox->addItem(query->value(1).toString(), query->value(0).toInt());
    }

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->selectRow(0);

    QObject::connect(ui->dateEdit,SIGNAL(dateChanged(QDate)),this,SLOT(setmin(QDate)));
}

void AddAbsence::setmin(QDate date)
{
    ui->dateEdit_2->setMinimumDate(date);
}


AddAbsence::~AddAbsence()
{
    delete ui;
}

void AddAbsence::AbsenceDialog()
{

    QString query_function;
    QDate date1, date2;
    int id_worker, id_absence_type;
    id_worker = ui->tableView->selectionModel()->selectedRows(3).value(0).data().toInt();
    id_absence_type = ui->comboBox->currentData().toInt();
    date1 = ui->dateEdit->date();
    date2 = ui->dateEdit_2->date();

    query_function = QString("SELECT schema1.add_absence(:worker_id,:id_absence_type,:date_from,:date_to)");
    QSqlQuery* query = new QSqlQuery(db);
    query->prepare(query_function);
    query->bindValue(":worker_id", id_worker);
    query->bindValue(":id_absence_type", id_absence_type);
    query->bindValue(":date_from", date1);
    query->bindValue(":date_to", date2);
    query->exec();

    qDebug() << query_function;
    qDebug() << db.lastError();
    qDebug() << query->lastError().text();

    if (query->lastError().isValid()) {
        ui->error_label->setStyleSheet("QLabel { color : red; }");
        std::string s = query->lastError().text().toUtf8().constData();;
        s = s.substr(0,s.find("(P0001)"));
        ui->error_label->setWordWrap(true);
        ui->error_label->setText(QString::fromUtf8(s.c_str()));
    }
    else {
        emit absence_inserted();
        emit accepted();
        AddAbsence::close();
    }
}

void AddAbsence::on_ok_button_clicked()
{
    AbsenceDialog();
}

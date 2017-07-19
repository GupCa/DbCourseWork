#include "hiringdialog.h"
#include "ui_hiringdialog.h"
#include "addphysical.h"
#include <QRegExpValidator>
#include <QDialog>
#include <QLineEdit>
#include <string>

HiringDialog::HiringDialog(QWidget *parent, QSqlDatabase db1) :
    QDialog(parent),
    ui(new Ui::HiringDialog)
{
    db = db1;
    ui->setupUi(this);

    ui->radioButton_new->setChecked(true);
    ui->radioButton_existed->setChecked(false);
    refresh_data_individual();
    ui->tableView_2->setEnabled(false);

    //ui->passport_lineEdit->setInputMask("AA 999999");
    //ui->medical_lineEdit->setInputMask("9999 9999 9999 9999");
    //ui->phone_edit_2->setInputMask("+38(O 99) 999 99 99");
    QRegExp rx ("[А-Яа-яіїєґІЇҐЄ]{1,20}");
    ui->name_edit_2->setValidator (new QRegExpValidator (rx, this));
    ui->surname_edit_2->setValidator (new QRegExpValidator (rx, this));
    ui->patronymic_edit_2->setValidator (new QRegExpValidator (rx, this));
    QRegExp rx1 ("[0-9]{9}");
    ui->phone_edit_2->setValidator (new QRegExpValidator (rx1, this));
    QRegExp rx2 ("[^@]{3,30}");
    ui->adress_lineEdit->setValidator (new QRegExpValidator (rx2, this));
    ui->education_lineEdit->setValidator(new QRegExpValidator (rx2, this));
    QRegExp rx3 ("[0-9]{16}");
    ui->medical_lineEdit->setValidator (new QRegExpValidator (rx3, this));
    QRegExp rx4 ("[А-Яа-яіїєґІЇҐЄ]{2}[0-9]{6}");
    ui->passport_lineEdit->setValidator (new QRegExpValidator (rx4, this));


    ui->date_edit_2->setMaximumDate(QDate::currentDate().addYears(-18));
    ui->date_edit_2->setMinimumDate(QDate::currentDate().addYears(-100));
    QString query_s;
    query_s = QString("select schema1.department.id_department, schema1.department.name FROM schema1.department;");
    QSqlQuery*  query = new QSqlQuery(db);
    query->exec(query_s);
    while (query->next())
    {
        ui->department_cb->addItem(query->value(1).toString(), query->value(0).toInt());
    }

    refresh_position_cb();

    QObject::connect(ui->department_cb, SIGNAL(currentIndexChanged(int)), this, SLOT(refresh_position_cb()));

    ui->tableView_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_2->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_2->selectRow(0);

    refresh_spin_slider();
    QObject::connect(ui->position_cb, SIGNAL(currentIndexChanged(int)), this, SLOT(refresh_spin_slider()));

    QObject::connect(ui->spinBox,SIGNAL(valueChanged(int)),ui->horizontalSlider,SLOT(setValue(int)));
    QObject::connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),ui->spinBox,SLOT(setValue(int)));
}

void HiringDialog::refresh_position_cb()
{
    ui->position_cb->clear();

    int id_department = ui->department_cb->currentData().toInt();
    QString query_s1;
    query_s1 = QString("WITH tableq AS( "
                       "SELECT schema1.worker_on_position.id_staffing_table as id_staff, "
                         "schema1.staffing_table.total_membership-count(schema1.worker_on_position.id_worker_on_position) AS available_amount_positions "
                        "FROM schema1.worker_on_position,schema1.staffing_table "
                        "WHERE schema1.worker_on_position.finish_date ISNULL "
                         "AND schema1.staffing_table.id_staffing_table = schema1.worker_on_position.id_staffing_table "
                       "GROUP BY schema1.worker_on_position.id_staffing_table, staffing_table.total_membership "
                       "UNION "
                       "SELECT schema1.staffing_table.id_staffing_table, schema1.staffing_table.total_membership "
                        "FROM schema1.staffing_table "
                        "WHERE staffing_table.id_staffing_table NOT IN (SELECT DISTINCT id_staffing_table FROM schema1.worker_on_position WHERE schema1.worker_on_position.finish_date ISNULL) "
                       ") "
                       "SELECT schema1.position.id_position, schema1.position.name_position FROM tableq, schema1.position, schema1.staffing_table  WHERE "
                         "position.id_department = '%1' AND "
                        "staffing_table.id_position = position.id_position AND staffing_table.id_staffing_table = tableq.id_staff AND tableq.available_amount_positions > 0;").arg(id_department);
    QSqlQuery* query = new QSqlQuery(db);
    query->exec(query_s1);

    while (query->next())
    {
        ui->position_cb->addItem(query->value(1).toString(), query->value(0).toInt());
    }

}

HiringDialog::~HiringDialog()
{
    delete ui;
}

void HiringDialog::refresh_data_individual()
{
    QSqlQueryModel *model = new QSqlQueryModel;
        model->setQuery("SELECT "
                        "DISTINCT schema1.individual.id_individual, "
                        "schema1.individual.surname, "
                        "schema1.individual.name, "
                        "schema1.worker.medical_reference, "
                        "schema1.worker.id_worker "
                      "FROM schema1.individual, schema1.worker, schema1.worker_on_position "
                      "WHERE "
                        "schema1.individual.id_individual = schema1.worker.id_individual AND "
                        "schema1.worker.id_worker = schema1.worker_on_position.id_worker AND "
                        "schema1.worker_on_position.finish_date NOTNULL AND "
                        "schema1.worker_on_position.id_worker NOT IN (SELECT schema1.worker_on_position.id_worker FROM schema1.worker_on_position WHERE finish_date ISNULL); " );
        model->setHeaderData(1, Qt::Horizontal, "призвіще");
        model->setHeaderData(2, Qt::Horizontal, "ім'я");
        model->setHeaderData(3, Qt::Horizontal, "страховка");


        ui->tableView_2->setModel(model);
        ui->tableView_2->setColumnHidden(0, true);
        ui->tableView_2->setColumnHidden(4, true);

}

void HiringDialog::refresh_spin_slider()
{
    int id_position = ui->position_cb->currentData().toInt();
    QString query_s2;
    query_s2 = QString("SELECT schema1.staffing_table.min_salary_position, schema1.staffing_table.max_salary_position "
                      "FROM schema1.staffing_table WHERE schema1.staffing_table.id_position = '%1'").arg(id_position);
    QSqlQuery* query = new QSqlQuery(db);
    query->exec(query_s2);
    query->next();
    ui->spinBox->setRange(query->value(0).toInt(), query->value(1).toInt());
    ui->horizontalSlider->setRange(query->value(0).toInt(), query->value(1).toInt());
}

void HiringDialog::on_Add_individual_clicked()
{
    auto dialog = new AddPhysical(0, db);
    dialog->show();

    QObject::connect(dialog, SIGNAL(individual_inserted()), this, SLOT(refresh_data_individual()));
}

void HiringDialog::on_pushButton_clicked()
{


    QRegExp rx ("[А-Яа-яіїєґІЇЄҐ]{1,20}");
    QRegExp rx1 ("[0-9]{9}");
    QRegExp rx2 ("[^@]{3,30}");
    QRegExp rx3 ("[0-9]{16}");
    QRegExp rx4 ("[А-Яа-яіїєґІЇЄҐ]{2}[0-9]{6}");

    ui->medical_lineEdit->setValidator (new QRegExpValidator (rx3, this));
    if (rx.exactMatch(ui->name_edit_2->text()) &&
        rx.exactMatch(ui->surname_edit_2->text()) &&
        rx.exactMatch(ui->patronymic_edit_2->text()) &&
        rx1.exactMatch(ui->phone_edit_2->text()) &&
        rx2.exactMatch(ui->adress_lineEdit->text()) &&
        rx2.exactMatch(ui->education_lineEdit->text()) &&
        rx3.exactMatch(ui->medical_lineEdit->text())  &&
        rx4.exactMatch(ui->passport_lineEdit->text()) ) {

        int salary, idposition;

        salary = ui->horizontalSlider->value();
        idposition = ui->position_cb->currentData().toInt();

        if (ui->radioButton_new->isChecked())  {


            QString name, surname, pathronymic, address, passport, education;
            QDate date;
            int phone;
            qint64 medical_reference;

            name = ui->name_edit_2->text();
            surname = ui->surname_edit_2->text();
            pathronymic = ui->patronymic_edit_2->text();
            phone = ui->phone_edit_2->text().toInt();
            date = ui->date_edit_2->date();
            address = ui->adress_lineEdit->text();
            passport = ui->passport_lineEdit->text();
            education = ui->education_lineEdit->text();
            medical_reference = ui->medical_lineEdit->text().toLongLong();

            QString query_f_withind;
            query_f_withind = QString("SELECT schema1.add_worker_withind(:name,:surname,:pathronymic,:date,:phone,:address,:passport,:education,:mr,:salary,:positionid)");
            QSqlQuery* query = new QSqlQuery(db);
            query->prepare(query_f_withind);
            query->bindValue(":name", name);
            query->bindValue(":surname", surname);
            query->bindValue(":pathronymic", pathronymic);
            query->bindValue(":date", date);
            query->bindValue(":phone", phone);
            query->bindValue(":address", address);
            query->bindValue(":passport", passport);
            query->bindValue(":education", education);
            query->bindValue(":mr", medical_reference);
            query->bindValue(":salary", salary);
            query->bindValue(":positionid", idposition);
            query->exec();

            qDebug() << query->lastError().text();

            if (query->lastError().isValid()) {
                ui->error_label->setStyleSheet("QLabel { color : red; }");
                std::string s = query->lastError().text().toUtf8().constData();;
                s = s.substr(0,s.find("(P0001)"));
                ui->error_label->setWordWrap(true);
                ui->error_label->setText(QString::fromUtf8(s.c_str()));
            }
            else {
                emit worker_hired();
                emit accepted();
                HiringDialog::close();
            }
        }


        else {
            qint64 medical;

            medical = ui->tableView_2->selectionModel()->selectedRows(3).value(0).data().toInt();

            QString query_function_id;
            query_function_id = QString("SELECT schema1.add_worker(:medical,:salary,:positionid)");
            QSqlQuery* query= new QSqlQuery(db);
            query->prepare(query_function_id);
            query->bindValue(":medical", medical);
            query->bindValue(":salary", salary);
            query->bindValue(":positionid", idposition);
            query->exec();

            qDebug() << query->lastError().text();

            if (query->lastError().isValid()) {
                ui->error_label->setStyleSheet("QLabel { color : red; }");
                std::string s = query->lastError().text().toUtf8().constData();;
                s = s.substr(0,s.find("(P0001)"));
                ui->error_label->setWordWrap(true);
                ui->error_label->setText(QString::fromUtf8(s.c_str()));
            }
            else {
                emit worker_hired();
                emit accepted();
                HiringDialog::close();
            }
        }
    }

    else {
        ui->error_label->setStyleSheet("QLabel { color : red; }");
        ui->error_label->setText("Введена назва некоректна");
    }
}

void HiringDialog::on_radioButton_new_toggled(bool checked)
{
    ui->name_edit_2->setEnabled(checked);
    ui->surname_edit_2->setEnabled(checked);
    ui->patronymic_edit_2->setEnabled(checked);
    ui->date_edit_2->setEnabled(checked);
    ui->phone_edit_2->setEnabled(checked);
    ui->adress_lineEdit->setEnabled(checked);
    ui->passport_lineEdit->setEnabled(checked);
    ui->education_lineEdit->setEnabled(checked);
    ui->medical_lineEdit->setEnabled(checked);

    if (checked) {
        ui->name_edit_2->clear();
        ui->surname_edit_2->clear();
        ui->patronymic_edit_2->clear();
        ui->passport_lineEdit->clear();
        ui->date_edit_2->setDate(QDate::currentDate().addYears(-35));
        ui->phone_edit_2->clear();
        ui->adress_lineEdit->clear();
        ui->passport_lineEdit->clear();
        ui->education_lineEdit->clear();
        ui->medical_lineEdit->clear();
    }
}


void HiringDialog::on_radioButton_existed_toggled(bool checked)
{
    ui->tableView_2->setEnabled(checked);
}

void HiringDialog::on_tableView_2_clicked(const QModelIndex &index)
{
    auto selectionModel = ui->tableView_2->selectionModel();
    if (!selectionModel->hasSelection())
    {
        return;
    }
    int id_individual = selectionModel->selectedRows(0).value(0).data().toInt();
    QString query_s2;
    query_s2 = QString("SELECT * FROM schema1.individual, schema1.worker WHERE schema1.individual.id_individual = '%1' AND worker.id_individual =  '%1'").arg(id_individual);
    QSqlQuery* query = new QSqlQuery(db);
    query->exec(query_s2);
    query->next();

    ui->name_edit_2->setText(query->value(2).toString());
    ui->surname_edit_2->setText(query->value(1).toString());
    ui->patronymic_edit_2->setText(query->value(3).toString());
    ui->date_edit_2->setDate(query->value(4).toDate());
    ui->phone_edit_2->setText(query->value(5).toString());
    ui->adress_lineEdit->setText(query->value(7).toString());
    ui->passport_lineEdit->setText(query->value(8).toString());
    ui->education_lineEdit->setText(query->value(9).toString());
    ui->medical_lineEdit->setText(query->value(10).toString());
}

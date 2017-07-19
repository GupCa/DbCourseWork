#include "mainwindow.h"
#include "loginwindow.h"
#include "ui_mainwindow.h"
#include "hiringdialog.h"
#include "addphysical.h"
#include "addabsence.h"
#include "dialogdepartament.h"
#include "dialogmovingstaff.h"
#include"dialogadddepartment.h"
#include <QObject>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QTableView>
#include <QtDebug>
#include <QtGui>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QResource>
#include <QMessageBox>
#include <QtCore>
#include <QTabWidget>
#include <QDialog>
#include <QTimer>
#include <stdlib.h>
#include <cstdlib> // now using C++ header
#include <cstdio>

#include <clocale>
#include "libxl.h"

using namespace libxl;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /*QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setDatabaseName("fc");
    db.setUserName("postgres");
    db.setPassword("++");

    if (!db.open())
            {
                    qDebug() << QObject::trUtf8("Бд не найдена") << db.lastError().text();
            }
    else{
        qDebug() << QObject::trUtf8("Бд найдена") << db.lastError().text();
    }

    QSqlTableModel* model = new QSqlTableModel;
    model->setTable("schema1.absence_type");
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->select();*/


    db = QSqlDatabase::addDatabase("QPSQL");
    showLoginWindow();
    role = getRole();
    if (role == "staff_manager") staffmanager();
    if (role == "inspector") inspector();
    if (role == "absence_checker") absencechecker();

   /* QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT schema1.position.name_position, schema1.worker_on_position.start_date,schema1.worker.passport "
                     "FROM schema1.position, schema1.worker_on_position, schema1.worker "
                     "WHERE schema1.position.id_position = schema1.worker_on_position.id_position "
                     "AND schema1.worker_on_position.id_worker = schema1.worker.id_worker");
    model->setHeaderData(0, Qt::Horizontal, "id");
    model->setHeaderData(1, Qt::Horizontal, "d");

    ui->tableView->setModel(model);*/
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::showLoginWindow(){
    QString El;
    while (1){
        loginWindow d(this, El);
        d.show();
        if(d.exec() == QDialog::Accepted){
            login = d.getLogin();
            pw = d.getPw();
            //user_type = d.getUserType();
            if (connectUser()){
                break;
                return 0;
            }

            else
                El = "<html><head/><body><p style=\"color:red;\">"
                     "Помилка доступу! <br>"
                     "Такого користувача немає в БД!</p></body></html>";
        }
        else{
            QApplication::closeAllWindows();
                    exit(0);
        }
    }
    return 1;
}

QString MainWindow::getRole(){
    QSqlQuery sq = db.exec(QString("select rolname from pg_user\
                            join pg_auth_members on (pg_user.usesysid=pg_auth_members.member)\
                            join pg_roles on (pg_roles.oid=pg_auth_members.roleid)\
                            where pg_user.usename='%1';").arg(login));

    sq.first();
    return sq.value(0).toString();
}

int MainWindow::connectUser(){
    db.setHostName(ip);
    db.setPort(port);
    db.setDatabaseName("fc");
    db.setUserName(login);
    db.setPassword(pw);
    if(db.open()){
        return 1;
    }
    qDebug() << db.lastError();
    return 0;
}

int MainWindow::staffmanager()
{
    ui->label->setText("Ви увійшли як керівник персоналом");

    ui->FireButton->setEnabled(true);
    ui->HireButton->setEnabled(true);
    ui->MoveButton->setEnabled(true);
    ui->addAbsence->setEnabled(true);
    ui->addDepartment->setEnabled(true);
    ui->AddPosition->setEnabled(true);

    ui->tab_2->setEnabled(true);
    ui->tab_4->setEnabled(true);

     QTimer *tmr;
     tmr = new QTimer();
     QObject::connect(tmr, SIGNAL(timeout()), this, SLOT(refresh_data_individual()));
     tmr->start(10000);

    refresh_data_workers();
    refresh_data_individual();
    refresh_data_absence();
    refresh_data_department();

    QString configwidget = ".QWidget {background-image: url(/Users/vlad/Documents/qt projects/staff.png)}";
    ui->centralWidget->setStyleSheet(configwidget);

    return 1;
}

int MainWindow::inspector()
{
    staffmanager();
    ui->label->setText("Ви увійшли як перевіряльник");
    ui->FireButton->setEnabled(false);
    ui->HireButton->setEnabled(false);
    ui->MoveButton->setEnabled(false);
    ui->addAbsence->setEnabled(false);
    ui->addDepartment->setEnabled(false);
    ui->AddPosition->setEnabled(false);
    ui->tab_2->setEnabled(true);
    ui->tab_4->setEnabled(true);
    QString configwidget = ".QWidget {background-image: url(/Users/vlad/Documents/qt projects/insp.jpg)}";
    ui->centralWidget->setStyleSheet(configwidget);
    return 1;
}

int MainWindow::absencechecker()
{
    staffmanager();
    ui->label->setText("Ви увійшли як відмічувач відсутності");
    ui->FireButton->setEnabled(true);
    ui->HireButton->setEnabled(true);
    ui->MoveButton->setEnabled(true);
    ui->addAbsence->setEnabled(true);
    ui->addDepartment->setEnabled(true);
    ui->AddPosition->setEnabled(true);
    ui->tab_2->setEnabled(false);
    ui->tab_4->setEnabled(false);
    QString configwidget = ".QWidget {background-image: url(/Users/vlad/Documents/qt projects/abscheck.jpg)}";
    ui->centralWidget->setStyleSheet(configwidget);
    return 1;
}


void MainWindow::refresh_data_workers()
{
    QSqlQueryModel *model = new QSqlQueryModel;
        model->setQuery("select * from schema1.worker_info;" );
        model->setHeaderData(0, Qt::Horizontal, "ім'я");
        model->setHeaderData(1, Qt::Horizontal, "призвіще");
        model->setHeaderData(2, Qt::Horizontal, "посада");

    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(3, true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->selectRow(0);
}

void MainWindow::refresh_data_department()
{
    QSqlQueryModel *model = new QSqlQueryModel;
        model->setQuery("SELECT * FROM schema1.department, schema1.position, schema1.staffing_table WHERE staffing_table.id_position = position.id_position AND "
                        "department.id_department = position.id_department;" );
        model->setHeaderData(1, Qt::Horizontal, "департамент");
        model->setHeaderData(5, Qt::Horizontal, "позиція");
        model->setHeaderData(8, Qt::Horizontal, "людей по плану");
        model->setHeaderData(9, Qt::Horizontal, "мінімальна зп");
        model->setHeaderData(10, Qt::Horizontal, "максимальна зп");

    ui->tableView_4->setModel(model);
    ui->tableView_4->setColumnHidden(0, true);
    ui->tableView_4->setColumnHidden(2, true);
    ui->tableView_4->setColumnHidden(3, true);
    ui->tableView_4->setColumnHidden(4, true);
    ui->tableView_4->setColumnHidden(6, true);
    ui->tableView_4->setColumnHidden(7, true);
}

void MainWindow::refresh_data_absence()
{
    QSqlQueryModel *model2 = new QSqlQueryModel;
    model2->setQuery("SELECT schema1.individual.name, schema1.individual.surname, schema1.position.name_position, schema1.absence.start_date, schema1.absence.finish_date, schema1.absence_type.type_desctription "
                    "FROM schema1.individual, schema1.position, schema1.worker, schema1.staffing_table, schema1.worker_on_position, schema1.absence, schema1.absence_type "
                    "WHERE schema1.position.id_position=schema1.staffing_table.id_position "
                    "AND schema1.staffing_table.id_staffing_table=schema1.worker_on_position.id_staffing_table "
                    "AND schema1.worker_on_position.id_worker=schema1.worker.id_worker "
                    "AND schema1.worker_on_position.finish_date is NULL "
                    "AND schema1.worker.id_individual=schema1.individual.id_individual "
                    "AND schema1.worker.id_worker = schema1.absence.id_worker "
                    "AND schema1.absence.id_absence_type=schema1.absence_type.id_absence_type; ");
    model2->setHeaderData(0, Qt::Horizontal, "ім'я");
    model2->setHeaderData(1, Qt::Horizontal, "призвіще");
    model2->setHeaderData(2, Qt::Horizontal, "посада");

    ui->tableView_3->setModel(model2);
}

void MainWindow::refresh_data_individual()
{
    QSqlTableModel* model1 = new QSqlTableModel;
    model1->setTable("schema1.individual");
    model1->select();
    //ui->tableView_2->setModel(model1);
    //ui->tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
}


void MainWindow::on_addAbsence_clicked()
{
    auto dialog = new AddAbsence(0, db);
    dialog->show();

    QObject::connect(dialog, SIGNAL(absence_inserted()), this, SLOT(refresh_data_absence()));
}

void MainWindow::on_MoveButton_clicked()
{
    int id_worker = ui->tableView->selectionModel()->selectedRows(3).value(0).data().toInt();
    auto dialog = new Dialogmovingstaff(0, db, id_worker);
    dialog->show();

    QObject::connect(dialog, SIGNAL(staff_moved()), this, SLOT(refresh_data_workers()));
}


void MainWindow::on_addDepartment_clicked()
{
    auto dialog = new Dialogadddepartment(0, db);
    dialog->show();
}

void MainWindow::on_FireButton_clicked()
{
    int id_worker = ui->tableView->selectionModel()->selectedRows(3).value(0).data().toInt();
    QString query_function;
    query_function = QString("SELECT schema1.fire_worker(:worker_id)");
    QSqlQuery* query = new QSqlQuery(db);
    query->prepare(query_function);
    query->bindValue(":worker_id", id_worker);
    query->exec();

    qDebug() << query_function;
    qDebug() << query->lastError().text();

    refresh_data_workers();
}

void MainWindow::on_AddPosition_clicked()
{
    auto dialog = new DialogDepartament(0, db);
    dialog->show();

    QObject::connect(dialog, SIGNAL(department_inserted()), this, SLOT(refresh_data_department()));
}

void MainWindow::on_HireButton_clicked()
{
    auto dialog = new HiringDialog(0, db);
    dialog->show();

    QObject::connect(dialog, SIGNAL(worker_hired()), this, SLOT(refresh_data_workers()));
}

/*void MainWindow::on_Add_individual_clicked()
{
    auto dialog = new AddPhysical(0, db);
    dialog->show();

    QObject::connect(dialog, SIGNAL(individual_inserted()), this, SLOT(refresh_data_individual()));
}*/

void MainWindow::on_relogin_Button_clicked()
{
    this->hide();
    showLoginWindow();
    role = getRole();
    if (role == "staff_manager") staffmanager();
    if (role == "inspector") inspector();
    if (role == "absence_checker") absencechecker();
    this->show();
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    if (index==1){
        refresh_data_absence();
    }
}

void MainWindow::on_add_report_Button_clicked()
{
    Book* book = xlCreateBook();
    book->setLocale("en_US.UTF-8");
    Sheet* sheet = book->addSheet("Sheet1");
    Format* dateFormat = book->addFormat();
    dateFormat->setNumFormat(NUMFORMAT_DATE);
    int rows = ui->tableView_3->verticalHeader()->count();

    for(int i=1;i<(rows+1);i++){
    sheet->writeStr(i, 0, ui->tableView_3->model()->index(i-1,0).data().toString().toStdString().c_str());
    sheet->writeStr(i, 1, ui->tableView_3->model()->index(i-1,1).data().toString().toStdString().c_str());
    sheet->writeStr(i, 2, ui->tableView_3->model()->index(i-1,2).data().toString().toStdString().c_str());
    sheet->writeNum(i, 3,book->datePack(ui->tableView_3->model()->index(i-1,3).data().toDate().year(), ui->tableView_3->model()->index(i-1,3).data().toDate().month(), ui->tableView_3->model()->index(i-1,3).data().toDate().day())
                    , dateFormat);
    sheet->writeNum(i, 4, book->datePack(ui->tableView_3->model()->index(i-1,4).data().toDate().year(), ui->tableView_3->model()->index(i-1,4).data().toDate().month(), ui->tableView_3->model()->index(i-1,4).data().toDate().day())
                    , dateFormat);
    sheet->writeStr(i, 5, ui->tableView_3->model()->index(i-1,5).data().toString().toStdString().c_str());
    auto str = QString("=E%1-D%1+1").arg(i+1);
    sheet->writeFormula(i, 6, str.toStdString().c_str());
    }

    sheet->writeNum(8, 6, book->datePack(2017, 5, 26), dateFormat);
    sheet->setCol(1, 1, 12);

    book->save("report.xls");

    book->release();

    ui->add_report_Button->setEnabled(false);

    QProcess::execute("open report.xls");
    ui->add_report_Button->setEnabled(true);

}

void MainWindow::on_pushButton_find_clicked()
{
    QString name, surname, nameposition, query;
    name = ui->lineEdit_findname->text();
    surname = ui->lineEdit_findsurname->text();
    nameposition = ui->lineEdit_findposition->text();


    QStringList slContr;
    query = QString("select * from schema1.worker_info WHERE name LIKE '%%1%' AND surname LIKE '%%2%' AND name_position LIKE '%%3%';").arg(name, surname, nameposition);
    qDebug() << query;
    QSqlQuery sq = db.exec(query);
    while (sq.next())
    slContr << sq.value(0).toString();
    qDebug() << slContr;
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery(query);
    ui->tableView->setModel(model);

}

void MainWindow::on_pushButton_refresh_clicked()
{
    refresh_data_workers();
}

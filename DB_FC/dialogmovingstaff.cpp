#include "dialogmovingstaff.h"
#include "ui_dialogmovingstaff.h"

Dialogmovingstaff::Dialogmovingstaff(QWidget *parent, QSqlDatabase db1, int id_worker1) :
    QDialog(parent),
    ui(new Ui::Dialogmovingstaff)
{
    ui->setupUi(this);
    db = db1;
    id_worker = id_worker1;
    QString query_s, name_position;
    query_s = QString("SELECT * from schema1.worker_info where id_worker =  '%1'").arg(id_worker);
    QSqlQuery* query = new QSqlQuery(db);
    query->exec(query_s);
    query->next();
    ui->infostaff_label->setText(query->value(0).toString()+" "+ query->value(1).toString()+" "+ query->value(2).toString());

    name_position = query->value(2).toString();
    query_s = QString("SELECT schema1.position.id_position FROM schema1.position WHERE schema1.position.name_position = '%1'").arg(name_position);
    query = new QSqlQuery(db);
    query->exec(query_s);
    query->next();

    int id_position = query->value(0).toInt();
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
                       "SELECT schema1.position.id_position, schema1.position.name_position FROM tableq, schema1.position, schema1.staffing_table  WHERE schema1.position.id_position != %1 "
                        "AND staffing_table.id_position = position.id_position AND staffing_table.id_staffing_table = tableq.id_staff AND tableq.available_amount_positions > 0;").arg(id_position);
    query = new QSqlQuery(db);
    query->exec(query_s1);

    while (query->next())
    {
        ui->position_comboBox->addItem(query->value(1).toString(), query->value(0).toInt());
    }

    refresh_spin_slider();
    QObject::connect(ui->position_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(refresh_spin_slider()));

    QObject::connect(ui->spinBox,SIGNAL(valueChanged(int)),ui->horizontalSlider,SLOT(setValue(int)));
    QObject::connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),ui->spinBox,SLOT(setValue(int)));

}

Dialogmovingstaff::~Dialogmovingstaff()
{
    delete ui;
}

void Dialogmovingstaff::refresh_spin_slider()
{
    int id_position = ui->position_comboBox->currentData().toInt();
    QString query_s;
    query_s = QString("SELECT schema1.staffing_table.min_salary_position, schema1.staffing_table.max_salary_position "
                      "FROM schema1.staffing_table WHERE schema1.staffing_table.id_position = '%1'").arg(id_position);
    QSqlQuery* query = new QSqlQuery(db);
    query->exec(query_s);
    query->next();
    ui->spinBox->setRange(query->value(0).toInt(), query->value(1).toInt());
    ui->horizontalSlider->setRange(query->value(0).toInt(), query->value(1).toInt());
}

void Dialogmovingstaff::on_pushButton_clicked()
{
    int id_position_to = ui->position_comboBox->currentData().toInt();
    int salary_to = ui->horizontalSlider->value();
    QString query_function;

    query_function = QString("SELECT schema1.move_worker(:id_worker, :id_position_to, :salary_to)");
    QSqlQuery* query = new QSqlQuery(db);
    query->prepare(query_function);
    query->bindValue(":id_worker", id_worker);
    query->bindValue(":id_position_to", id_position_to);
    query->bindValue(":salary_to", salary_to);
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
        emit staff_moved();
        emit accepted();
        Dialogmovingstaff::close();
    }
}

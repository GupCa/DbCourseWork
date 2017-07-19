#ifndef HIRINGDIALOG_H
#define HIRINGDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QtSql>

namespace Ui {
class HiringDialog;
}

class HiringDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HiringDialog(QWidget *parent = 0, QSqlDatabase db1 = QSqlDatabase());
    ~HiringDialog();

signals:
    void worker_hired();

private slots:
    void on_Add_individual_clicked();
    void refresh_data_individual();
    void refresh_position_cb();
    void refresh_spin_slider();
    void on_pushButton_clicked();


    void on_radioButton_new_toggled(bool checked);

    void on_radioButton_existed_toggled(bool checked);

    void on_tableView_2_clicked(const QModelIndex &index);

private:
    Ui::HiringDialog *ui;
    QSqlDatabase db;
};

#endif // HIRINGDIALOG_H

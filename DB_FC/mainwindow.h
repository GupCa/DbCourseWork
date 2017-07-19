#include <QMainWindow>
#include <QtSql>
#include <QtDebug>
#include <QTableWidget>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QSqlDatabase db;

private slots:
    void refresh_data_absence();
    void refresh_data_individual();
    void refresh_data_department();
    void refresh_data_workers();
    void on_HireButton_clicked();
    void on_addAbsence_clicked();
    void on_MoveButton_clicked();
    void on_addDepartment_clicked();

    void on_FireButton_clicked();

    void on_AddPosition_clicked();

    void on_relogin_Button_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_add_report_Button_clicked();

    void on_pushButton_find_clicked();

    void on_pushButton_refresh_clicked();

private:
    Ui::MainWindow *ui;
    QString login;
    QString pw;
    QString ip = "localhost";
    int port = 5432;
    QString role;


    QString getRole();
    int connectUser();
    int showLoginWindow();
    int staffmanager();
    int inspector();
    int absencechecker();

protected:


};

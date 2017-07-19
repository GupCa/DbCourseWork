#include "loginwindow.h"
#include "ui_loginwindow.h"

#include <QCompleter>
#include <QDialogButtonBox>
#include <QDialog>
#include <QDesktopWidget>
#include <QLineEdit>
#include <QStringList>


loginWindow::loginWindow(QWidget *parent, QString El) :
    QDialog(parent),
    ui(new Ui::loginWindow)
{
    setModal(true);
        ui->setupUi(this);
        ui->error_label->setText(El);
        QStringList wordList;

        wordList << "insp_1" << "insp_2" << "abs_check_1" << "abs_check_2" << "stf_mng_1";
        QCompleter *completer = new QCompleter(wordList, this);
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        ui->login_edit->setCompleter(completer);

       // connect( ui->enter_button, SIGNAL( clicked() ), SLOT( accept() ) );
       // connect( ui->exit_button, SIGNAL( clicked() ), SLOT( reject()  ) );

        ui->login_edit->setText("stf_mng_1");
        ui->password_edit->setText("stf_mng_1");
        ui->password_edit->setEchoMode(QLineEdit::Password);


        QDesktopWidget desktop;
        QRect rect = desktop.availableGeometry(desktop.primaryScreen()); // прямоугольник с размерами экрана
        QPoint center = rect.center(); //координаты центра экрана
        int x = center.x() - (this->width()/2);  // учитываем половину ширины окна
        int y = center.y() - (this->height()/2); // .. половину высоты
        center.setX(x);
        center.setY(y);
        this->move(center);

}

loginWindow::~loginWindow()
{
    delete ui;
}

QString loginWindow::getLogin()
{
    return ui->login_edit->text();
}

QString loginWindow::getPw()
{
    return ui->password_edit->text();
}


/*void loginWindow::paintEvent(QPaintEvent *) {
    QImage img("C:/megan_2.png");
    QPainter painter(this);
    painter.drawImage(0,0, img.scaled(this->size()));
    //ui->name_label->setStyleSheet("color: rgb(255,000,000)");
    //ui->logwin_label->setStyleSheet("color: rgb(255,000,000)")
    ui->log_label->setStyleSheet("color: rgb(255,250,250)");
    ui->pass_label->setStyleSheet("color: rgb(255,250,250)");
  //  ui->ip_label->setStyleSheet("color: rgb(255,250,250)");
  //  ui->port_label->setStyleSheet("color: rgb(255,250,250)");
   // ui->type_label->setStyleSheet("color: rgb(255,250,250)");
   // ui->hr_button->setStyleSheet("color: rgb(255,250,250)");
   // ui->merch_button->setStyleSheet("color: rgb(255,250,250)");
    ui->error_label->setStyleSheet("color: rgb(255,250,250)");
}*/

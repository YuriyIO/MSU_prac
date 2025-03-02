#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


bool str_is_num(QString str)
{
    if(str.size() == 0)
        return false;
    if(str[0] == '0')
        return false;

    for(int i = 0; i < (int)str.size(); i++)
    {
        if(!('0' <= str[i] && str[i] <= '9'))
        {
            return false;
        }
    }

    return true;
}



void MainWindow::on_pushButton_quit_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"", "Are you sure to close the app?", QMessageBox::Yes | QMessageBox::No);

    if(reply == QMessageBox::Yes)
    {
        QApplication::quit();
    }
}

bool check_input_data(QString room_nums[], QString room_prices[], QString room_num,QString day_num)
{
    bool check = true;

    for(int i = 0; (i < 5) && (check != false); i++)
    {
        check &=  str_is_num(room_nums[i]);
        check &=  str_is_num(room_prices[i]);
    }

    check &=  str_is_num(room_num);
    check &=  str_is_num(day_num);

    if(check)
    {
        check &=  (12 <= day_num.toInt()) && (day_num.toInt() <= 30);
    }

    for(int i = 0; (i < 5) && (check != false); i++)
    {
        check = (50 <= room_prices[i].toInt()) && (room_prices[i].toInt() <= 500);
    }


    int sum_rooms = 0;
    for(int i = 0; (i < 5) && (check != false); i++)
    {
        sum_rooms += room_nums[i].toInt();
    }

    check &= sum_rooms == room_num.toInt();

    return check;
}


void MainWindow::on_pushButton_continue_clicked()
{
    if(!this->is_init)//нет инициализации => проверяем входные данные и создаем эксперимент
    {
        bool check = false;

        QString room_nums[5];
        QString room_prices[5];
        QString room_num = ui->lineEdit_room_num->text();
        QString day_num = ui->lineEdit_day_num->text();


        room_nums[0] = ui->lineEdit_n1->text();
        room_nums[1] = ui->lineEdit_n2->text();
        room_nums[2] = ui->lineEdit_n3->text();
        room_nums[3] = ui->lineEdit_n4->text();
        room_nums[4] = ui->lineEdit_n5->text();

        room_prices[0] = ui->lineEdit_p1->text();
        room_prices[1] = ui->lineEdit_p2->text();
        room_prices[2] = ui->lineEdit_p3->text();
        room_prices[3] = ui->lineEdit_p4->text();
        room_prices[4] = ui->lineEdit_p5->text();


        if(str_is_num(ui->lineEdit_room_num->text()))
        {
            check = check_input_data(room_nums, room_prices, room_num, day_num);
        }

        check &= str_is_num(ui->lineEdit_req_num->text());

        if(!check)// неверные данные => сообщение об ощибке
        {
            QMessageBox::warning(this, "", "Please enter valid data");
        }
        else//инициализация
        {
            int room_nums_int[MAX_NUM];
            int room_prices_int[MAX_NUM];
            for(int i = 0; i < MAX_NUM; i++)
            {
                room_nums_int[i] = room_nums[i].toInt();
                room_prices_int[i] = room_prices[i].toInt();
            }
            this->is_init = true;
            this->exp.exp_init(day_num.toInt(), room_num.toInt(), room_nums_int, room_prices_int);
            this->exp.set_max_req_num(ui->lineEdit_req_num->text().toInt());

            //вывод информации об отеле
            int i = 0;
            QString q_str;
            for(int j = SINGLE_ROOM; j < MAX_NUM; j++)
            {
                q_str = QString::fromStdString(this->exp.get_room_type(i)) + "(s):";
                ui->listWidget_hotel->addItem(q_str);
                for(;this->exp.get_room_class(i) == j; i++)
                {
                    q_str = QString::fromStdString(this->exp.get_hotel_room_info(i));
                    ui->listWidget_hotel->addItem(q_str);
                }
                ui->listWidget_hotel->addItem("");
            }
            ui->pushButton_continue->setText("Продолжить");


            //запрет на изменение введенных параметров
            ui->lineEdit_n1->setReadOnly(true);
            ui->lineEdit_n2->setReadOnly(true);
            ui->lineEdit_n3->setReadOnly(true);
            ui->lineEdit_n4->setReadOnly(true);
            ui->lineEdit_n5->setReadOnly(true);

            ui->lineEdit_p1->setReadOnly(true);
            ui->lineEdit_p2->setReadOnly(true);
            ui->lineEdit_p3->setReadOnly(true);
            ui->lineEdit_p4->setReadOnly(true);
            ui->lineEdit_p5->setReadOnly(true);

            ui->lineEdit_day_num->setReadOnly(true);
            ui->lineEdit_room_num->setReadOnly(true);
            ui->lineEdit_req_num->setReadOnly(true);
        }
    }
    else//эксперемент уже начат
    {
        ui->listWidget_hotel->clear();
        ui->listWidget_req->clear();

        if(this->exp.get_cur_day() < this->exp.get_amount_of_days())//не последний день
        {
            int req_num = this->exp.gen_req_num();
            QString q_str;
            string req;
            for(int i = 0; i < req_num; i++)//обработка запросов и вывод информации о них
            {
                this->exp.run_one_req(req);
                q_str = QString::fromStdString(req);
                ui->listWidget_req->addItem(q_str);
            }

            if(req_num == 0)
            {
                q_str = "We have no requests";
                ui->listWidget_req->addItem(q_str);

            }

            //вывод информации об отеле
            int i = 0;
            for(int j = SINGLE_ROOM; j < MAX_NUM; j++)
            {
                q_str = QString::fromStdString(this->exp.get_room_type(i)) + "(s):";
                ui->listWidget_hotel->addItem(q_str);
                for(;this->exp.get_room_class(i) == j; i++)
                {
                    q_str = QString::fromStdString(this->exp.get_hotel_room_info(i,this->exp.get_cur_day()));
                    ui->listWidget_hotel->addItem(q_str);
                }
                ui->listWidget_hotel->addItem("");
            }

            ui->label_cur_day->setNum(this->exp.get_cur_day());
            ui->label_income->setNum(this->exp.get_hotel_income());
            this->exp.inc_cur_day();
        }
        else//последний день
        {
            //вывод информации об отеле
            QString q_str;
            int i = 0;
            for(int j = SINGLE_ROOM; j < MAX_NUM; j++)
            {
                q_str = QString::fromStdString(this->exp.get_room_type(i)) + "(s):";
                ui->listWidget_hotel->addItem(q_str);
                for(;this->exp.get_room_class(i) == j; i++)
                {
                    q_str = QString::fromStdString(this->exp.get_hotel_room_info(i));
                    ui->listWidget_hotel->addItem(q_str);
                }
                ui->listWidget_hotel->addItem("");
            }

            //вывод статистики
            q_str = "Количество обработанных запросов: " + QString::number(this->exp.get_num_proc_req());
            ui->listWidget_req->addItem(q_str);
            q_str = "Количество проваленных запросов: " + QString::number(this->exp.get_num_faild_req());
            ui->listWidget_req->addItem(q_str);
            q_str = "Полученный доход: " + QString::number(this->exp.get_hotel_income());
            ui->listWidget_req->addItem(q_str);
            q_str = "Максимально возможный доход: " + QString::number(this->exp.get_max_income());
            ui->listWidget_req->addItem(q_str);


        }

    }

}


void MainWindow::on_pushButton_to_end_clicked()
{
    ui->listWidget_hotel->clear();
    ui->listWidget_req->clear();

    if(!this->is_init)//нет инициализации => проверяем входные данные и создаем эксперимент
    {
        bool check = false;

        QString room_nums[5];
        QString room_prices[5];
        QString room_num = ui->lineEdit_room_num->text();
        QString day_num = ui->lineEdit_day_num->text();


        room_nums[0] = ui->lineEdit_n1->text();
        room_nums[1] = ui->lineEdit_n2->text();
        room_nums[2] = ui->lineEdit_n3->text();
        room_nums[3] = ui->lineEdit_n4->text();
        room_nums[4] = ui->lineEdit_n5->text();

        room_prices[0] = ui->lineEdit_p1->text();
        room_prices[1] = ui->lineEdit_p2->text();
        room_prices[2] = ui->lineEdit_p3->text();
        room_prices[3] = ui->lineEdit_p4->text();
        room_prices[4] = ui->lineEdit_p5->text();


        if(str_is_num(ui->lineEdit_room_num->text()))
        {
            check = check_input_data(room_nums, room_prices, room_num, day_num);
        }

        check &= str_is_num(ui->lineEdit_req_num->text());

        if(!check)
        {
            QMessageBox::warning(this, "", "Please enter valid data");
        }
        else
        {
            int room_nums_int[MAX_NUM];
            int room_prices_int[MAX_NUM];
            for(int i = 0; i < MAX_NUM; i++)
            {
                room_nums_int[i] = room_nums[i].toInt();
                room_prices_int[i] = room_prices[i].toInt();
            }
            this->is_init = true;
            this->exp.exp_init(day_num.toInt(), room_num.toInt(), room_nums_int, room_prices_int);
            this->exp.set_max_req_num(ui->lineEdit_req_num->text().toInt());


            ui->pushButton_continue->setText("Продолжить");

            ui->lineEdit_n1->setReadOnly(true);
            ui->lineEdit_n2->setReadOnly(true);
            ui->lineEdit_n3->setReadOnly(true);
            ui->lineEdit_n4->setReadOnly(true);
            ui->lineEdit_n5->setReadOnly(true);

            ui->lineEdit_p1->setReadOnly(true);
            ui->lineEdit_p2->setReadOnly(true);
            ui->lineEdit_p3->setReadOnly(true);
            ui->lineEdit_p4->setReadOnly(true);
            ui->lineEdit_p5->setReadOnly(true);

            ui->lineEdit_day_num->setReadOnly(true);
            ui->lineEdit_room_num->setReadOnly(true);
            ui->lineEdit_req_num->setReadOnly(true);
        }
    }
    else
    {
        //проход по всем оставшимся дням и обработка запросов
        for(int i = this->exp.get_cur_day(); i < this->exp.get_amount_of_days(); i++)
        {

            int req_num = this->exp.gen_req_num();
            string req;
            for(int i = 0; i < req_num; i++)
            {
                this->exp.run_one_req(req);
            }

            this->exp.inc_cur_day();
        }

        ui->label_cur_day->setNum(this->exp.get_cur_day());
        ui->label_income->setNum(this->exp.get_hotel_income());


        //вывод информации об отеле и статистики
        QString q_str;
        int i = 0;
        for(int j = SINGLE_ROOM; j < MAX_NUM; j++)
        {
            q_str = QString::fromStdString(this->exp.get_room_type(i)) + "(s):";
            ui->listWidget_hotel->addItem(q_str);
            for(;this->exp.get_room_class(i) == j; i++)
            {
                q_str = QString::fromStdString(this->exp.get_hotel_room_info(i));
                ui->listWidget_hotel->addItem(q_str);
            }
            ui->listWidget_hotel->addItem("");
        }


        q_str = "Количество обработанных запросов: " + QString::number(this->exp.get_num_proc_req());
        ui->listWidget_req->addItem(q_str);
        q_str = "Количество проваленных запросов: " + QString::number(this->exp.get_num_faild_req());
        ui->listWidget_req->addItem(q_str);
        q_str = "Полученный доход: " + QString::number(this->exp.get_hotel_income());
        ui->listWidget_req->addItem(q_str);
        q_str = "Максимально возможный доход: " + QString::number(this->exp.get_max_income());
        ui->listWidget_req->addItem(q_str);
    }


}



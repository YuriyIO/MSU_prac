#ifndef HOTEL_CODE_H
#define HOTEL_CODE_H

#include <vector>
#include <string>

using namespace std;

enum class_room
{
    SINGLE_ROOM,
    DOUBLE_ROOM,
    BIG_DOUBLE_ROOM,
    HALF_VIP_ROOM,
    VIP_ROOM,
    MAX_NUM
};


class Room
{
private:
    vector<bool> days_info;
    class_room type_of_room;


public:

    Room(class_room type_of_room, const int days);

    class_room get_room_type();

    bool get_day_info(int day);//свободна комната в этот день или нет

    int set_day_info(int day_start, int day_end);

    string room_free_days(int cur_day = 0);//все дни, когда комната свободна, начиная с полученной даты

    int count_free_days();//количество свободных дней
};


class Guest_request
{
public:

    class_room room_class;
    int day_start;
    int day_end;

    void generate(const int cur_day, const int last_day);
};



class Hotel
{
private:

    vector<Room> all_rooms;
    int room_price[MAX_NUM];
    string const room_names[MAX_NUM] = {"single room", "double room", "big double room", "half-vip room", "vip room"};
    int income = 0;
    int max_income = 0;
    int num_faild_req = 0;
    int num_proc_req = 0;


public:

    void hotel_init(const int rooms, const int days, int room_nums[], int room_prices[]);

    string book_room_info(const Guest_request my_req);//обработка запроса и возвращение информации об этом

    int book_room(int day_start, int day_end, class_room type_of_room);//резервирование комнаты

    class_room get_room_class(int num);

    string get_room_type(int num);

    string str_room_info(int num, int cur_day = 0);//вывод информации об выбранной комнатк

    int get_income();

    int get_max_income();

    int get_num_faild_req();//количество отклоненных запросов

    int get_num_proc_req();//количество выполненых запросов

    int room_free_days(int num);
};




class Experiment
{
private:

    int amount_of_days;
    int amount_of_rooms;
    int cur_day = 0;
    int max_req_num = 8;
    Guest_request my_req;
    Hotel host;


public:

    int get_max_income();

    int get_num_faild_req();

    int get_num_proc_req();

    int count_all_free_room_days();

    void exp_init(const int days, const int rooms, int room_nums[], int room_prices[]);

    int get_amount_of_rooms();

    int get_amount_of_days();

    void set_max_req_num(int num);

    class_room get_room_class(int num);

    string get_room_type(int num);

    string get_hotel_room_info(int num, int cur_day = 0);

    int get_hotel_income();

    int get_cur_day();

    int gen_req_num();

    void run_one_req(string &req_info);

    void inc_cur_day();
};







#endif // HOTEL_CODE_H

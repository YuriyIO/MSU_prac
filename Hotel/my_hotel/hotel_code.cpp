#include "hotel_code.h"


string to_str_num(int i)
{
    string res = to_string(i);
    if(i < 10)
        res += "  ";
    return res;
}


Room::Room(class_room type_of_room, const int days)
{
    this->type_of_room = type_of_room;
    days_info = vector<bool> (days);
    for(int i = 0; i < days; i++)
    {
        days_info[i] = 0;
    }
}



class_room Room::get_room_type()
{
    return type_of_room;
}

bool Room::get_day_info(int day)
{
    if(day >= (int)days_info.size())
    {
        return 0;
    }
    return days_info[day];
}

int Room::set_day_info(int day_start, int day_end)
{
    if(max(day_start, day_end) > (int) days_info.size())
    {
        return -1;
    }

    for(int i = day_start; i < day_end; i++)
    {
        days_info[i] = 1;
    }
    return 0;
}

int Room::count_free_days()
{
    int res = 0;
    for(int i = 0; i < (int)days_info.size(); i++)
    {
        res += days_info[i] == 0;
    }
    return res;
}


string Room::room_free_days(int cur_day)
{
    string res = "";
    for(int i = cur_day; i < (int)days_info.size(); i++)
    {
        if(!days_info[i])
            res += to_string(i) + " ";

    }
    return res;
}


void Guest_request::generate(const int cur_day, const int last_day)
{
    room_class = class_room(rand()%(MAX_NUM));

    int last = rand()%(last_day - cur_day);
    if(last == 0)
        last = 1;

    day_start = cur_day;
    day_end = day_start + last;
}


void Hotel::hotel_init(const int rooms, const int days, int room_nums[], int room_prices[])
{
    for(int i = 0; i < MAX_NUM; i++)
    {
        room_price[i] = room_prices[i];
    }

    for(int j = SINGLE_ROOM; j < MAX_NUM; j++)
    {
        for(int i = 0; i < room_nums[j]; i++)
        {
            all_rooms.push_back(Room((class_room)j, days));
        }
    }
}



int Hotel::book_room(int day_start, int day_end, class_room type_of_room)
{
    max_income += room_price[type_of_room]*(day_end - day_start);
    int num = 0;
    while(all_rooms[num].get_room_type() != type_of_room)
    {
        num++;
    }

    int sum;
    for(; num < (int)all_rooms.size(); num++)
    {
        sum = 0;
        for(int d = day_start; d < day_end; d++)
        {
            sum += all_rooms[num].get_day_info(d);
        }
        if(sum == 0)
            break;
    }
    if(sum != 0)
    {
        num_faild_req ++;
        return -1;
    }

    int res = all_rooms[num].set_day_info(day_start, day_end);
    if(res == -1)
    {
        num_faild_req ++;
        return -1;
    }

    num_proc_req ++;

    income += room_price[type_of_room]*(day_end - day_start);

    return num;
}

class_room Hotel::get_room_class(int num)
{
    return all_rooms[num].get_room_type();
}

string Hotel::get_room_type(int num)
{
    return room_names[all_rooms[num].get_room_type()];
}

string Hotel::book_room_info(const Guest_request my_req)
{
    string res = "Request: start = " + to_string(my_req.day_start) + "; end = " + to_string(my_req.day_end) + "; room class = " + room_names[my_req.room_class] + "\n";
    int book_res = book_room(my_req.day_start, my_req.day_end, my_req.room_class);
    if(book_res < 0)
        res += "Faild";
    else
        //res += all_rooms[book_res].get_room_name() + " " + to_string(book_res) + " for " +  to_string(room_price[my_req.room_class]) + " per day";
        res += "Room " + to_string(book_res) + ". price == " + to_string(room_price[my_req.room_class]);
    return res;
}

string Hotel::str_room_info(int num, int cur_day)
{
    return "room " + to_str_num(num) + " is free: " +  all_rooms[num].room_free_days(cur_day);
}

int Hotel::room_free_days(int num)
{
    return all_rooms[num].count_free_days();
}


int Hotel::get_income()
{
    return income;
}

int Hotel::get_max_income()
{
    return max_income;
}

int Hotel::get_num_faild_req()
{
    return num_faild_req;
}

int Hotel::get_num_proc_req()
{
    return num_proc_req;
}



int Experiment::count_all_free_room_days()
{
    int res = 0;
    for(int i = 0; i < amount_of_rooms; i++)
    {
        res += host.room_free_days(i);
    }
    return res;
}


void Experiment::exp_init(const int days, const int rooms, int room_nums[], int room_prices[])
{
    amount_of_days = days;
    amount_of_rooms = rooms;
    host.hotel_init(rooms, days, room_nums, room_prices);
}

int Experiment::get_amount_of_rooms()
{
    return amount_of_rooms;
}

int Experiment::get_amount_of_days()
{
    return amount_of_days;
}

int Experiment::get_max_income()
{
    return host.get_max_income();
}

int Experiment::get_num_faild_req()
{
    return host.get_num_faild_req();
}

int Experiment::get_num_proc_req()
{
    return host.get_num_proc_req();
}

void Experiment::set_max_req_num(int num)
{
    max_req_num = num;
}


class_room Experiment::get_room_class(int num)
{
    return host.get_room_class(num);
}

string Experiment::get_room_type(int num)
{
    return host.get_room_type(num);
}

string Experiment::get_hotel_room_info(int num, int cur_day)
{
    if(num < amount_of_rooms)
        return host.str_room_info(num, cur_day);
    return "";
}


int Experiment::get_hotel_income()
{
    return host.get_income();
}

int Experiment::get_cur_day()
{
    return cur_day;
}


int Experiment::gen_req_num()
{
    return rand()%max_req_num;
}

void Experiment::run_one_req(string &req_info)
{
    req_info = "";
    my_req.generate(cur_day, amount_of_days);
    req_info += host.book_room_info(my_req);
}


void Experiment::inc_cur_day()
{
    cur_day++;
}




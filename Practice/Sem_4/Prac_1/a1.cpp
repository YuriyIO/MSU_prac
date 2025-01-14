#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<iomanip>

using namespace std;

class Student;


class Day_of_birth
{   
    friend Student;
private:
    int day;
    int month;
    int year;
public:
    Day_of_birth(int Day = 0, int Month = 0, int Year = 0)
    {day = Day; month = Month; year = Year;}
    Day_of_birth& operator = (Day_of_birth Birth)
    {day = Birth.day; month = Birth.month; year = Birth.year; return *this;}
    bool operator == (Day_of_birth Birth)
    {if(day == Birth.day && month == Birth.month && year == Birth.year) return true; return false;}
    bool operator != (Day_of_birth Birth)
    {if(day == Birth.day && month == Birth.month && year == Birth.year) return false; return true;}
    bool operator < (Day_of_birth Birth)
    {
        if(year < Birth. year)
            return true;
        if(year > Birth. year)
            return false;
        if(month < Birth. month)
            return true;
        if(month > Birth. month)
            return false;
        if(day < Birth.day)
            return true;
        if(day > Birth.day)
            return false;
        return false;
        
    }
};

class Student
{
private:
    string name;
    string  surname;
    Day_of_birth birth;
    long long phone_number;
public:
    Student(Day_of_birth Birth, string Name = "none", string Surname = "none", long long Mobile_phone = 0) 
    {name = Name; surname = Surname; birth = Birth; phone_number = Mobile_phone;};
    Student(string Name = "none", string Surname = "none", long long Mobile_phone = 0)
    {name = Name; surname = Surname; birth.day = birth.month = birth.year = 0; phone_number = Mobile_phone;} 
    void print()
    {
        if(name!="none")
            cout << "Name: "<< name << endl;
        if(surname !="none")
            cout << "Surname: "<< surname << endl;
        if(phone_number != 0)
            cout << "Phone number: "<< phone_number << endl;
        if(birth.day != 0 && birth.month != 0 && birth.year != 0)
                cout << "Date of birth: " << setfill('0') << setw(2) << birth.day << "." << setfill('0') << setw(2) << birth.month << "." << birth.year << endl;
    }
    string get_name(){return name;}
    string get_surname(){return surname;}
    long long get_phone_number(){return phone_number;}
    Day_of_birth get_Day_of_birth(){return birth;};
    void change_name(string new_name){name = new_name;};
    void change_surname(string new_surname){surname = new_surname;};
    void change_phone_number(long long new_phone_number){phone_number = new_phone_number;};
    void chande_Day_of_birth(int Day = 0, int Month = 0, int Year = 0){birth.day = Day; birth.month = Month; birth.year = Year;;}
    bool operator == (Student st)
    {if(name == st.name && surname == st.surname && phone_number == st.phone_number && birth == st.birth) return true; return false;}
};
class Group
{
private:
    int Group_number;
    Student* Group_of_students = nullptr;
    int array_size = 0;
    int number_of_students = 0;
public:
    Group(int number = 0)
    {
        Group_number = number;
        Group_of_students = new Student [array_size = 10];
        Group_of_students[0] = Student();
    };
    ~Group(){if(Group_of_students != nullptr) delete[] Group_of_students;};
    void print()
    {
        if(number_of_students == 0) cout << "There aren't any students\n" << endl;
        for(int i = 1; i <= number_of_students; i++)
        {
            cout << "Number: " << i << endl;
            Group_of_students[i].print();
            cout << endl;
        }
    };
    void set_group_number(int number){Group_number = number;};
    void sort_by_name()
    {
        for(int i = 1; i <= number_of_students; i++)
            for(int j = number_of_students; j > i; j--)
                if(Group_of_students[j].get_name() < Group_of_students[j - 1].get_name() && Group_of_students[j - 1].get_name() != "None" && Group_of_students[j].get_name() != "None")
                {
                    Student t = Group_of_students[j]; Group_of_students[j] = Group_of_students[j-1]; Group_of_students[j-1] = t;
                }
        cout << "\nWe sorted group by names\n" << endl;
    };
    void sort_by_surname()
    {
        for(int i = 1; i <= number_of_students; i++)
            for(int j = number_of_students; j > i; j--)
                if(Group_of_students[j].get_surname() < Group_of_students[j - 1].get_surname() && Group_of_students[j - 1].get_surname() != "None" && Group_of_students[j].get_surname() != "None")
                {
                    Student t = Group_of_students[j]; Group_of_students[j] = Group_of_students[j-1]; Group_of_students[j-1] = t;
                }
        cout << "\nWe sorted group by surnames\n" << endl;
    };
    void sort_by_phone_number()
    {
        for(int i = 1; i <= number_of_students; i++)
            for(int j = number_of_students; j > i; j--)
                if(Group_of_students[j].get_phone_number() < Group_of_students[j - 1].get_phone_number() && Group_of_students[j - 1].get_phone_number() != 0 && Group_of_students[j].get_phone_number() != 0)
                {
                    Student t = Group_of_students[j]; Group_of_students[j] = Group_of_students[j-1]; Group_of_students[j-1] = t;
                }
        cout << "\nWe sorted group by phone number\n" << endl;
    };
    void sort_by_birth()
    {
        Day_of_birth zero;
        for(int i = 1; i <= number_of_students; i++)
            for(int j = number_of_students; j > i; j--)
                if(Group_of_students[j].get_Day_of_birth() < Group_of_students[j - 1].get_Day_of_birth() && Group_of_students[j - 1].get_Day_of_birth() != zero && Group_of_students[j].get_Day_of_birth() != zero)
                {
                    Student t = Group_of_students[j]; Group_of_students[j] = Group_of_students[j-1]; Group_of_students[j-1] = t;
                }
        cout << "\nWe sorted group by birth\n" << endl;
    };
    void add_student(Student st)
    {
        if(number_of_students >= array_size - 1)
        {
            Student* tmp = Group_of_students;
            Group_of_students = new Student [array_size *= 2];
            for(int i = 0; i < number_of_students; i++)
                Group_of_students[i] = tmp[i];
            delete[] tmp;
        }
        Group_of_students[++number_of_students] = st;
    };
    void fire_stutent(Student st)
    {
        int st_num = number_of_students + 100;
        for(int i = 1; i <= number_of_students; i++)
            if(Group_of_students[i] == st)
                st_num = i;
        if(st_num <= number_of_students)
        {
            for(int i = st_num ; i < number_of_students; i++)
                Group_of_students[i] = Group_of_students[i+1];
            number_of_students--;
        }
    };
    /*Student& find_student(Student st)
    {
        int student_count = 0;
        int max_sing = 0;
        int sing = 0;
        int std_num;
        Day_of_birth zero_birth;
        for(int i = 1; i <= number_of_students; i++)
        {
            sing = 0;
            if(st.get_name() ==  Group_of_students[i].get_name() && st.get_name() != "none")
                sing++;
            if(st.get_surname() ==  Group_of_students[i].get_surname() && st.get_surname()!= "none")
                sing++;
            if(st.get_phone_number() ==  Group_of_students[i].get_phone_number() && st.get_phone_number() != 0)
                sing++;
            if(st.get_Day_of_birth() ==  Group_of_students[i].get_Day_of_birth() && st.get_Day_of_birth()  != zero_birth)
                sing++;
            if(sing > max_sing)
            {
                std_num = i;
                max_sing = sing;
                student_count = 1;
            }
            else if(sing == max_sing && sing > 0)
                student_count++;
        }
        if(student_count == 1)
        {
            cout << "\nWe find this student\n" << endl;
            return  Group_of_students[std_num];
        }
        if(student_count > 1)
            cout << "We have " << student_count << " the same students" << endl;
        else cout << "We do not have such a student" << endl;
        return Group_of_students[0];
    }*/
    Student* find_student(Student st)
    {
        int student_count = 0;
        int max_sing = 0;
        int sing = 0;
        int std_num;
        Day_of_birth zero_birth;
        for(int i = 1; i <= number_of_students; i++)
        {
            sing = 0;
            if(st.get_name() ==  Group_of_students[i].get_name() && st.get_name() != "none")
                sing++;
            if(st.get_surname() ==  Group_of_students[i].get_surname() && st.get_surname()!= "none")
                sing++;
            if(st.get_phone_number() ==  Group_of_students[i].get_phone_number() && st.get_phone_number() != 0)
                sing++;
            if(st.get_Day_of_birth() ==  Group_of_students[i].get_Day_of_birth() && st.get_Day_of_birth()  != zero_birth)
                sing++;
            if(sing > max_sing)
            {
                std_num = i;
                max_sing = sing;
                student_count = 1;
            }
            else if(sing == max_sing && sing > 0)
                student_count++;
        }
        if(student_count == 1)
        {
            cout << "\nWe find this student" << endl;
            return  &Group_of_students[std_num];
        }
        if(student_count > 1)
            cout << "\nWe have " << student_count << " the same students\n" << endl;
        else cout << "\nWe do not have such a student\n" << endl;
        return &Group_of_students[0];
    }

};

void f2(Student* st2);

void f1(Group& m1)
{
    Student st, st_zero;
    Student* st2;
    int t = 1;
    cout << endl;
    while (t != 0 && t != 7 && t != 8 && t != 9)
    {
        cout << "Choose an option:" << endl;
        cout << "\t1: set name" << endl;
        cout << "\t2: set surname" << endl;
        cout << "\t3: set phone number" << endl;
        cout << "\t4: set day of birth" << endl;
        cout << "\t5: set all" << endl;
        cout << "\t6: print student's data" << endl;
        cout << "\t7: add student" << endl;
        cout << "\t8: fire student" << endl;
        cout << "\t9: find student" << endl;
        cout << "\t0: cancel" << endl;
        cout << "Option number: ";
        cin >> t;
        if (t == 1)
        {
            string s;
            cout << "Name: ";
            cin >> s;
            st.change_name(s);
            cout << endl;
        }
        else if (t == 2)
        {
            string s;
            cout << "Surname: ";
            cin >> s;
            st.change_surname(s);
            cout << endl;
        }
        else if (t == 3)
        {
            long long p;
            cout << "Phone number: ";
            cin >> p;
            st.change_phone_number(p);
            cout << endl;
        }
        else if (t == 4)
        {
            int day, month, year;
            cout << "Day: ";
            cin >> day;
            cout << "Month: ";
            cin >> month;
            cout << "Year: ";
            cin >> year;
            st.chande_Day_of_birth(day, month, year);
            cout << endl;
        }
        else if (t == 5)
        {
            string s; long long p; int day, month, year;
            cout << "Name: "; cin >> s;
            st.change_name(s);
            cout << "Surname: "; cin >> s;
            st.change_surname(s);
            cout << "Phone number: ";cin >> p;
            st.change_phone_number(p);
            cout << "Day: "; cin >> day;
            cout << "Month: "; cin >> month;
            cout << "Year: "; cin >> year;
            st.chande_Day_of_birth(day, month, year);
            cout << endl;
        }
        else if (t == 6)
        {
            cout << endl;
            st.print();
            cout << endl;
        }
        else if (t == 7)
        {
            m1.add_student(st);
            cout << "\nWe add the student\n" << endl;
        }
        else if (t == 8)
        {
            m1.fire_stutent(st);
            if(st == st_zero) cout << "\nThis student has no data\n" << endl;
            else cout << "\nWe fire the student\n" << endl;
        }
        else if (t == 9)
        {
            st2 = m1.find_student(st);
            if((*st2) == st_zero)
                t = 0;
            else f2(st2);
        }
    }
}

void f2(Student* st2)
{
    int t = 1;
    cout << endl;
    while (t != 0)
    {
        cout << "Choose an option:" << endl;
        cout << "\t1: reset name" << endl;
        cout << "\t2: reset surname" << endl;
        cout << "\t3: reset phone number" << endl;
        cout << "\t4: reset day of birth" << endl;
        cout << "\t5: reset all" << endl;
        cout << "\t6: print student's data" << endl;
        cout << "\t0: leave" << endl;
        cout << "Option number: ";
        cin >> t;
        if (t == 1)
        {
            string s;
            cout << "Name: ";
            cin >> s;
            (*st2).change_name(s);
            cout << endl;
        }
        else if (t == 2)
        {
            string s;
            cout << "Surname: ";
            cin >> s;
            (*st2).change_surname(s);
            cout << endl;
        }
        else if (t == 3)
        {
            long long p;
            cout << "Phone number: ";
            cin >> p;
            (*st2).change_phone_number(p);
            cout << endl;
        }
        else if (t == 4)
        {
            int day, month, year;
            cout << "Day: ";
            cin >> day;
            cout << "Month: ";
            cin >> month;
            cout << "Year: ";
            cin >> year;
            (*st2).chande_Day_of_birth(day, month, year);
            cout << endl;
        }
        else if (t == 5)
        {
            string s; long long p; int day, month, year;
            cout << "Name: "; cin >> s;
            (*st2).change_name(s);
            cout << "Surname: "; cin >> s;
            (*st2).change_surname(s);
            cout << "Phone number: ";cin >> p;
            (*st2).change_phone_number(p);
            cout << "Day: "; cin >> day;
            cout << "Month: "; cin >> month;
            cout << "Year: "; cin >> year;
            (*st2).chande_Day_of_birth(day, month, year);
            cout << endl;
        }
        else if (t == 6)
        {
            cout << endl;
            (*st2).print();
            cout << endl;
        }
    }

}


void menu()
{
    Group m1;
    int t = 1;
    while(t)
    {
        cout << "Choose an option:" << endl;
        cout << "\t1: deal with students"<<endl;
        cout << "\t2: sort group by name"<<endl;
        cout << "\t3: sort group by surname"<<endl;
        cout << "\t4: sort group by phone numeber"<<endl;
        cout << "\t5: sort group by the day of birth"<<endl;
        cout << "\t6: print members of group"<<endl;
        cout << "\t0: exit"<<endl;
        cout << "Option number: "; cin >> t;
        if(t == 1) f1(m1);
        if(t == 2){ m1.sort_by_name();}
        if(t == 3){m1.sort_by_surname();}
        if(t == 4){m1.sort_by_phone_number();}
        if(t == 5){m1.sort_by_birth();}
        if(t == 6){cout<< endl; m1.print();}
    }
}

int main()
{
    menu();

    return 0;
}
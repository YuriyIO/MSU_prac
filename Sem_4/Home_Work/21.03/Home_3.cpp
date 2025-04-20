#include <iostream>
#include<list>
#include<vector>

using namespace std;

template <typename T>
void create_list(list<T>& l, const vector<T> v)
{
    for(size_t i = 0; i < v.size(); i++)
        if(v.at(i) > 0)
            l.push_back(v.at(i));
}

template <typename T>
T aver_num(const list<T>& l)
{
    T res = 0;
    for(auto iter = l.begin(); iter != l.end(); iter++)
        res += *iter;
    return res/l.size();
}

template <typename T>
void add_aver_num(list<T>& l)
{
    l.push_back(aver_num(l));
}

template <typename T>
void print_list(const list<T>& l)
{
    for(auto iter = l.begin(); iter != l.end(); iter++)
        cout<< *iter <<endl;
}


void test_int()
{
    vector<int> v = {-1, 2, -2, -8, 6, 0, 8};
    list<int> l;
    
    create_list(l, v);
    print_list(l);
    cout << endl;
    add_aver_num(l);
    print_list(l);

}

void test_double()
{
    vector<double> v = {1.2, -3.2, 2.4};
    list<double> l;
    
    create_list(l, v);
    print_list(l);
    cout << endl;
    add_aver_num(l);
    print_list(l);

}


int main()
{
    test_int();

    cout << endl;

    test_double();

    return 0;
}
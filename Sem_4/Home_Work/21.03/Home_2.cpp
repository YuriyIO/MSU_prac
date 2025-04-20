#include <iostream>
#include<list>
#include<vector>

using namespace std;

double aver_num(const list<double>& l)
{
    double res = 0;
    for(auto iter = l.begin(); iter != l.end(); iter++)
        res += *iter;
    return res/l.size();
}

void add_aver_num(list<double>& l)
{
    l.push_back(aver_num(l));
}

void print_list(const list<double>& l)
{
    for(auto iter = l.begin(); iter != l.end(); iter++)
        cout<< *iter <<endl;
}

int main()
{
    list<double> l = {5, 4, 2};
    add_aver_num(l);
    print_list(l);


    return 0;
}
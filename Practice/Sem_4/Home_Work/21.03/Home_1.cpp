#include <iostream>
#include<list>
#include<vector>

using namespace std;


void create_list(list<int>& l, const vector<int> v)
{
    for(size_t i = 0; i < v.size(); i++)
        if(v.at(i) > 0)
            l.push_back(v.at(i));
}


int main()
{
    vector<int> v = {-1, 2, 3, 6, -2, -8, 6, 0, 8};
    list<int> l;
    create_list(l, v);
    for(auto iter = l.begin(); iter != l.end(); iter++)
        cout<< *iter <<endl;



    return 0;
}
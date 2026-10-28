#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;


int main()
{
    vector<int> coll = {1,2,3,4,5,6,7,8,9};

    vector<int>::const_iterator pos;

    pos = find(coll.cbegin(),coll.cend(),3);

    cout<<"pos :"<<*pos<<endl;

    vector<int>::const_reverse_iterator rpos;

    rpos = find(coll.crbegin(),coll.crend(),3);
   
    cout<<"rpos :"<<*rpos<<endl;
    return 0;
}
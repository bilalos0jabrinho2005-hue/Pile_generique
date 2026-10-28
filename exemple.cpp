#include <iostream>
#include <vector>
using namespace std;

int main()
{
    vector<int> v = {1,2,3};
    v.insert(v.begin(),0);
    auto it = v.begin();
    cout<<*it;
    return 0;
}    
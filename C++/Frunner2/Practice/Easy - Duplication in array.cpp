#include<bits/stdc++.h>
using namespace std;

int main()
{
    int x;
    vector<int> a;
    while(cin>>x)
        a.push_back(x);
    vector<int> t;
    bool check = false;
    for(int i:a)
        if(find(t.begin(),t.end(),i)==t.end())
        {
            t.push_back(i);
        }
        else
            check = true;
    if(check == false)
        cout<<0;
    else
        cout<<1;
}

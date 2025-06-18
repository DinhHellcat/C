#include<bits/stdc++.h>
using namespace std;

int main()
{
    int x;
    vector<int> a;
    while(cin>>x)
        a.push_back(x);
    x=0;
    for(int i:a)
        if(i!=0)
        {
            cout<<i<<" ";
        }
        else
            x++;
    for(int i=0;i<x;i++)
        cout<<0<<" ";
}

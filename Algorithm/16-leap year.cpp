#include<bits/stdc++.h>
using namespace std;
int main()
{
    int y;
    cin>>y;
    if(0<y && y<100000)
    {
        if((y%4==0 && y%100!=0) || y%400==0)
            cout<<"YES";
        else
            cout<<"NO";
    }
    else
        cout<<"INVALID";
}

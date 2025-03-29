#include<bits/stdc++.h>
using namespace std;
int main()
{
    float x,y;
    cin>>x>>y;
    cout<<x+y<<endl;
    cout<<x-y<<endl;
    cout<<x*y<<endl;
    if(y!=0)
        cout<<fixed<<setprecision(2)<<x/y;
    else
        cout<<"ERROR";
}

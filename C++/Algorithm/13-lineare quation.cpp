#include<bits/stdc++.h>
using namespace std;
int main()
{
    float a,b;
    cin>>a>>b;
    if(a==0)
    {
        if(b==0)
            cout<<"WOW";
        else
            cout<<"NO";
    }
    else
        cout<<fixed<<setprecision(2)<<-b/a;
}

#include<bits/stdc++.h>
using namespace std;
int main()
{
    float a,b,c;
    cin>>a>>b>>c;
    if(a==0)
    {
        if(b==0)
        {
            if(c==0)
                cout<<"WOW";
            else
                cout<<"NO";
        }
        else
            cout<<fixed<<setprecision(2)<<-c/b;
    }
    else
    {
        float delta = pow(b,2)-4*a*c;
        if(delta<0)
            cout<<"NO";
        else if(delta==0)
            cout<<fixed<<setprecision(2)<<-b/(a*2);
        else
            cout<<fixed<<setprecision(2)<<(-b-sqrt(delta))/(2*a)<<" "<<(-b+sqrt(delta))/(2*a);
    }
}

#include<bits/stdc++.h>
using namespace std;
int main()
{
    float x,y;
    char c;
    cin>>x>>c>>y;
    switch(c)
    {
        case '+':{cout<<x+y;break;}
        case '-':{cout<<x-y;break;}
        case '*':{cout<<x*y;break;}
        case '/':
        {
            if(y==0)
                cout<<"Math Error";
            else
                cout<<fixed<<setprecision(2)<<x/y;break;
        }
    }
}

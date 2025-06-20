#include<bits/stdc++.h>
using namespace std;
int main()
{
    float a,b,c,d,e,f;
    cin>>a>>b>>c>>d;
    e=abs(c-a);
    f=abs(d-b);
    cout<<fixed<<setprecision(2)<<sqrt(e*e+f*f);
}

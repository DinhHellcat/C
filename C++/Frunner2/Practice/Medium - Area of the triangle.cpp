#include<bits/stdc++.h>
using namespace std;
int main()
{
    double a,b,c;
    cin>>a>>b>>c;
    if(a+b<c||a+c<b||a>b+c||a<=0||b<=0||c<=0)
        cout<<-1;
    else
    {
        double s = (a+b+c)/2;
        cout<<fixed<<setprecision(2)<<sqrt(s*(s-a)*(s-b)*(s-c));
    }
}

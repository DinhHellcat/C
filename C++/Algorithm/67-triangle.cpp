#include<bits/stdc++.h>
using namespace std;
int main()
{
    double a,b,c;
    cin>>a>>b>>c;
    if(a+b<c||a+c<b||a>b+c||a<=0||b<=0||c<=0)
        cout<<"NO";
    else
    {
        double p=(a+b+c)/2;
        double h=2*sqrt(p*(p-a)*(p-b)*(p-c))/max(max(a,b),c);
        cout<<a+b+c;
        cout<<fixed<<setprecision(2)<<" "<<0.5*max(max(a,b),c)*h;
    }
}

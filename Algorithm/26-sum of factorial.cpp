#include<bits/stdc++.h>
using namespace std;
double giaithua(double x)
{
    if(x==0) return 1;
    return x*giaithua(x-1);
}
double sum(double x,double n)
{
    if(n==1) return x;
    return pow(x,n)/giaithua(n) + sum(x,n-1);
}
int main()
{
    double a,b;
    cin>>a>>b;
    cout<<fixed<<setprecision(2)<<sum(a,b);
}

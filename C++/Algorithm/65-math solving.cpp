#include<bits/stdc++.h>
using namespace std;
double sum2(double x)
{
    if(x==0) return 0;
    return x+sum2(x-1);
}
double sum1(double x)
{
    if(x==0) return 0;
    return 1/sum2(x)+sum1(x-1);
}
int main()
{
    int n;
    cin>>n;
    double a[n],x;
    for(int i=0;i<n;i++)
    {
        cin>>x;
        a[i]=2*x/(x+1);
    }
    for(int i=0;i<n;i++)
        cout<<fixed<<setprecision(8)<<a[i]<<endl;
}

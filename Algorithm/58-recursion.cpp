#include<bits/stdc++.h>
using namespace std;
double sum(double a)
{
    if(a==0) return 0;
    return 1/((a+1)*a) + sum(a-1);
}
int main()
{
    double a;
    cin>>a;
    cout<<fixed<<setprecision(5)<<sum(a);
}

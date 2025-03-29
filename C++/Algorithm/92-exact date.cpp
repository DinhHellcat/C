#include<bits/stdc++.h>
using namespace std;
int main()
{
    int n,y;
    cin>>n>>y;
    int d=0,m=1;
    int a[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if(y%400==0 || (y%100!=0 && y%4==0))
        a[1]=29;
    while(n>0)
    {
        if(n<a[m-1])
        {
            d=n;
            n=0;
        }
        else
        {
            n-=a[m-1];
            m++;
        }
    }
    cout<<d<<" "<<m;
}

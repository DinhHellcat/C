#include<bits/stdc++.h>
using namespace std;
int main()
{
    long n;
    cin>>n;
    long a[n];
    for(int i=0;i<n;i++)
        cin>>a[i];
    double x=0,t=0;
    for(int i=0;i<n;i++)
        if(abs(a[i])%2==1)
        {
            t+=a[i];
            x++;
        }
    cout<<fixed<<setprecision(4)<<t/x;
}

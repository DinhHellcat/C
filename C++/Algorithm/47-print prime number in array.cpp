#include<bits/stdc++.h>
using namespace std;
bool ngto(long n)
{
    if(n<=1) return false;
    for(int i=2;i<=sqrt(n);i++)
        if(n%i==0)
            return false;
    return true;
}
int main()
{
    long n;
    cin>>n;
    long a[n];
    for(int i=0;i<n;i++)
        cin>>a[i];
    sort(a,a+n);
    for(int i=0;i<n;i++)
        if(a[i]!=a[i-1] && ngto(a[i]))
            cout<<a[i]<<" ";
}

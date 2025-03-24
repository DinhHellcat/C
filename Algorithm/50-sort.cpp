#include<bits/stdc++.h>
using namespace std;
int main()
{
    long n,t;
    cin>>n;
    long a[n];
    for(int i=0;i<n;i++)
        cin>>a[i];
    sort(a,a+n);
    t=a[n-1]-a[0];
    cout<<t;
}

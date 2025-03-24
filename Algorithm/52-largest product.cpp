#include<bits/stdc++.h>
using namespace std;
int main()
{
    long n;
    cin>>n;
    long a[n];
    for(int i=0;i<n;i++)
        cin>>a[i];
    sort(a,a+n);
    if(a[n-1]*a[n-2]>a[0]*a[1])
        cout<<a[n-1]*a[n-2];
    else
        cout<<a[0]*a[1];
}

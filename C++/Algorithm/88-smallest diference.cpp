#include<bits/stdc++.h>
using namespace std;
int main()
{
    int n;
    cin>>n;
    long a[n];
    for(int i=0;i<n;i++)
        cin>>a[i];
    sort(a,a+n);
    int t = a[1]-a[0];
    for(int i=1;i<n-1;i++)
        if(a[i+1]-a[i]<t)
            t=a[i+1]-a[i];
    cout<<t;
}

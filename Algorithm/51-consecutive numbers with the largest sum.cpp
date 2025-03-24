#include<bits/stdc++.h>
using namespace std;
int main()
{
    long n,t;
    cin>>n;
    long a[n],b[2];
    for(int i=0;i<n;i++)
        cin>>a[i];
    b[0]=a[0]; b[1]=a[n-1]; t=a[0]+a[n-1];
    for(int i=0;i<n-1;i++)
        if(a[i]+a[i+1]>t)
        {
             b[0]=a[i]; b[1]=a[i+1]; t=a[i]+a[i+1];
        }
    cout<<b[0]<<" "<<b[1];
}

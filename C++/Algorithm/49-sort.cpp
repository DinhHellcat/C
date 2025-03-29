#include<bits/stdc++.h>
using namespace std;
int main()
{
    long n,t;
    cin>>n;
    long a[n];
    for(int i=0;i<n;i++)
        cin>>a[i];
    for(int i=1;i<n;i++)
        for(int j=i+1;j<n-1;j++)
            if(a[i]>a[j])
            {
                t=a[i];
                a[i]=a[j];
                a[j]=t;
            }
    for(int i=0;i<n;i++)
        cout<<a[i]<<" ";
}

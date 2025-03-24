#include<bits/stdc++.h>
using namespace std;
int main()
{
    long n;
    cin>>n;
    long a[n];
    for(int i=0;i<n;i++)
        cin>>a[i];
    for(int i=0;i<n;i++)
        if(i%2==1)
        {
            if(i==n-1)
                a[i]+=abs(0-a[i-1]);
            else
                a[i]+=abs(a[i-1]-a[i+1]);
        }
    for(int i=0;i<n;i++)
        cout<<a[i]<<" ";
}

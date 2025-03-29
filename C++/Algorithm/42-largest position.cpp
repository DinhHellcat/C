#include<bits/stdc++.h>
using namespace std;
int main()
{
    long n;
    cin>>n;
    long a[n];
    for(int i=0;i<n;i++)
        cin>>a[i];
    long max=a[0],x=0;
    for(int i=1;i<n;i++)
        if(a[i]>=max)
        {
            max=a[i];
            x=i;
        }
    cout<<x;
}

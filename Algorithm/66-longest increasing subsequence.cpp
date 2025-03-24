#include<bits/stdc++.h>
using namespace std;
int main()
{
    long n,t=0,max=0,x=0;
    cin>>n;
    long a[n],b[10001];
    for(int i=0;i<n-1;i++)
        cin>>a[i];
    for(int i=0;i<n-1;i++)
    {
        for(int j=i;j<n;j++)
            if(a[j]>b[x-1])
            {
                t++;
                x++;
            }
        if(t>max)
            max=t;
        t=0;x=0;
    }
    cout<<max;
}

#include<bits/stdc++.h>
using namespace std;
long sum(long n)
{
    long s=0;
    for(long i=1;i<=n/i;i++)
    {
        if(n%i==0)
            s+=i+n/i;
        if(pow(i,2)==n)
            s-=i;
    }
    return s;
}
int main()
{
    int n;
    cin>>n;
    long a[n],x;
    for(int i=0;i<n;i++)
    {
        cin>>x;
        a[i]=sum(x);
    }
    for(int i=0;i<n;i++)
        cout<<a[i]<<endl;
}

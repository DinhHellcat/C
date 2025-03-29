#include<bits/stdc++.h>
using namespace std;
int major(int n,const auto& a)
{
    int count=1;
    long majornum=a[0];
    for(long i=1;i<n;i++)
    {
        if(count==0)
        {
            majornum=a[i];
            count=1;
        }
        else if(majornum==a[i])
            count++;
        else
            count--;
    }
    return majornum;
}
int main()
{
    int n;
    cin>>n;
    vector<long> a(n);
    for(int i=0;i<n;i++)
        cin>>a[i];
    cout<<major(n,a);
}

#include<bits/stdc++.h>
using namespace std;

int main()
{
    long long n;
    cin>>n;
    vector<long long> a(n);
    for(long long i=0;i<n;i++)
        cin>>a[i];

    long long t=INT_MIN;
    for(long long i=0;i<n;i++)
    {
        long long s=1;
        for(long long j=i;j<n;j++)
        {
            s*=a[j];
            if(s>t)
                t=s;
        }
    }
    cout<<t;
}

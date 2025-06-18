#include<bits/stdc++.h>
using namespace std;

int main()
{
    int n;
    cin>>n;
    vector<int> a(n);
    for(int i=0;i<n;i++)
        cin>>a[i];
    int nho=0;
    a[n-1]+=1;
    for(int i=n-1;i>=0;i--)
    {
        if(nho==1)
        {
            a[i]+=1;
            nho=0;
        }
        if(a[i]==10)
        {
            a[i]=0;
            nho=1;
        }
    }
    if(a[0]==0)
    {
        cout<<1<<" ";
        a[0]=0;
    }
    for(int i:a)
        cout<<i<<" ";
}

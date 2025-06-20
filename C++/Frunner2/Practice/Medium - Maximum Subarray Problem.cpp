#include<bits/stdc++.h>
using namespace std;
int main()
{
    int n;
    cin>>n;
    vector<int> a(n);
    for(int i=0;i<n;i++)
        cin>>a[i];
    if(n==0)
        cout<<0;
    else if(n==1)
        cout<<a[0];
    else
    {
        int t,x;
        t=x=a[0];
        for(int i=1;i<n;i++)
        {
            t=max(a[i],t+a[i]);
            x=max(t,x);
        }
        cout<<x;
    }
}

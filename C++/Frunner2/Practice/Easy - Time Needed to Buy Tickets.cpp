#include<bits/stdc++.h>
using namespace std;

int main()
{
    int n;
    cin>>n;
    vector<int> a(n);
    for(int i=0;i<n;i++)
        cin>>a[i];
    int k;
    cin>>k;
    int time=0;
    for(int i=0;i<n;i++)
    {
        if(i<=k)
            time+=min(a[i],a[k]);
        else
            time+=min(a[i],a[k]-1);
    }
    cout<<time;
}

#include<bits/stdc++.h>
using namespace std;

int main()
{
    int n,m;
    cin>>n>>m;
    vector<int> a(n+m);
    for(int i=0;i<n+m;i++)
        cin>>a[i];
    for(int i=n;i<n+m;i++)
        cin>>a[i];
    sort(a.begin(),a.end());
    for(int i:a)
        cout<<i<<" ";
}

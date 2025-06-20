#include<bits/stdc++.h>
using namespace std;
int main()
{
    int n,m,x;
    cin>>n;
    vector<int> a;
    for(int i=0;i<n;i++)
    {
        cin>>x;
        a.push_back(x);
    }
    cin>>m;
    for(int i=0;i<m;i++)
    {
        cin>>x;
        a.push_back(x);
    }
    sort(a.begin(),a.end());
    for(int i:a)
        cout<<i<<" ";
}

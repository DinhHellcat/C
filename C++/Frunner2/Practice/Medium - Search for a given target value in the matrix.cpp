#include<bits/stdc++.h>
using namespace std;
bool check(vector<vector<int>>& a,int x,int n)
{
    for(int i=0;i<n;i++)
        if(find(a[i].begin(),a[i].end(),x)!=a[i].end())
            return true;
    return false;
}
int main()
{
    int n,m;
    cin>>n>>m;
    vector<vector<int>> a(n,vector<int>(m));
    for(int i=0;i<n;i++)
        for(int j=0;j<m;j++)
            cin>>a[i][j];
    int x;
    cin>>x;
    cout<<check(a,x,n);
}

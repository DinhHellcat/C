#include<bits/stdc++.h>
using namespace std;
bool check(vector<vector<int>>& a,int n)
{
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            if(a[i][j]!=a[j][i])
                return false;
    return true;
}
int main()
{
    int n;
    cin>>n;
    vector<vector<int>> a(n,vector<int>(n));
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            cin>>a[i][j];
    cout<<check(a,n);
}

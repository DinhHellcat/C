#include<bits/stdc++.h>
using namespace std;
int main()
{
    int n,m,k,s=0;
    cin>>n>>m>>k;
    vector<vector<int>> a(n,vector<int>(m));
    for(int i=0;i<n;i++)
        for(int j=0;j<m;j++)
            cin>>a[i][j];
    for(int i=0;i<m;i++)
    {
        s+=a[k][i];
    }
    cout<<s;
}

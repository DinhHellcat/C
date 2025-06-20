#include<bits/stdc++.h>
using namespace std;
int main()
{
    int n,m,s=0;
    cin>>n>>m;
    vector<vector<int>> a(n,vector<int>(m));
    for(int i=0;i<n;i++)
        for(int j=0;j<m;j++)
        {
            cin>>a[i][j];
            s+=a[i][j];
        }
    cout<<s;
}

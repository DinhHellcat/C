#include<bits/stdc++.h>
using namespace std;
int main()
{
    int n,m;
    cin>>n>>m;
    vector<vector<int>> a(n,vector<int>(n));
    for(int i=0;i<n;i++)
        for(int j=0;j<m;j++)
            cin>>a[i][j];

    for(int j=0;j<m;j++)
    {
        for(int i=n-1;i>=0;i--)
            cout<<a[i][j]<<" ";
        cout<<endl;
    }
}

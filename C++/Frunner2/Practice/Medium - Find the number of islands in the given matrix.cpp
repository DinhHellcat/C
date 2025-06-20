#include<bits/stdc++.h>
using namespace std;
int n,m;
void dequi(vector<vector<int>>& t,int a, int b)
{
    if(a>=n || b>=m || a<0 || b<0)
        return;
    if(t[a][b]==0)
        return;
    t[a][b]=0;
    dequi(t,a+1,b);
    dequi(t,a,b+1);
    dequi(t,a-1,b);
    dequi(t,a,b-1);
}
int main()
{
    cin>>n>>m;
    vector<vector<int>> a(n,vector<int>(m));
    for(int i=0;i<n;i++)
        for(int j=0;j<m;j++)
            cin>>a[i][j];

    int t=0;
    for(int i=0;i<n;i++)
        for(int j=0;j<m;j++)
            if(a[i][j]==1)
            {
                t++;
                dequi(a,i,j);
            }
    cout<<t;
}

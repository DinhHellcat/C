#include<bits/stdc++.h>
using namespace std;
int main()
{
    int x,y,n;
    cin>>x>>y>>n;
    vector<vector<int>> matran(x,vector<int>(y));
    for(int i=0;i<x;i++)
        for(int j=0;j<y;j++)
            cin>>matran[i][j];
    sort(matran[n-1].begin(), matran[n-1].end());
    for(const auto& row : matran)
    {
        for(int val : row)
            cout<<val<<" ";
        cout<<endl;
    }
}

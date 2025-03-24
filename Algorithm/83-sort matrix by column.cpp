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
    vector<int> temp;
    for(int i=0;i<matran.size();i++)
        temp.push_back(matran[i][n-1]);
    sort(temp.begin(), temp.end());
    for(int i=0;i<matran.size();i++)
        matran[i][n-1]=temp[i];
    for(const auto& row : matran)
    {
        for(int val : row)
            cout<<val<<" ";
        cout<<endl;
    }
}

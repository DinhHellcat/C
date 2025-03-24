#include<bits/stdc++.h>
using namespace std;
int main()
{
    int x,y;
    cin>>x>>y;
    int a[x][y];
    for(int i=0;i<x;i++)
        for(int j=0;j<y;j++)
            cin>>a[i][j];
    for(int i=0;i<x;i++)
    {
        for(int j=0;j<y;j++)
            cout<<a[i][j]<<" ";
        cout<<endl;
    }
}

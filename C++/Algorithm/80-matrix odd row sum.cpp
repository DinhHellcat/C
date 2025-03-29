#include<bits/stdc++.h>
using namespace std;
int main()
{
    int x,y,sum=0;
    cin>>x>>y;
    int a[x][y];
    for(int i=0;i<x;i++)
        for(int j=0;j<y;j++)
            cin>>a[i][j];
    for(int i=1;i<x;i+=2)
        for(int j=0;j<y;j++)
            sum+=a[i][j];
    cout<<sum;
}

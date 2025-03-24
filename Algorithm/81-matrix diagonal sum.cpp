#include<bits/stdc++.h>
using namespace std;
int main()
{
    int x,sum=0;
    cin>>x;
    int a[x][x];
    for(int i=0;i<x;i++)
        for(int j=0;j<x;j++)
            cin>>a[i][j];
    for(int i=0;i<x;i++)
        sum+=a[i][i];
    cout<<sum;
}

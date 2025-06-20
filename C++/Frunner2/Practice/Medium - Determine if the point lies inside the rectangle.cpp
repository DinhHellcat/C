#include<bits/stdc++.h>
using namespace std;
int main()
{
    int a,b,c,d,x,y;
    cin>>x>>y>>a>>b>>c>>d;
    if(x>=min(a,c) && x<=max(a,c) && y>=min(b,d) && y<=max(b,d))
        cout<<1;
    else
        cout<<0;
}

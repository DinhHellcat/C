#include<bits/stdc++.h>
using namespace std;
int main()
{
    long x1,x2,x3,x4;
    long y1,y2,y3,y4;
    cin>>x1>>x2>>x3>>x4>>y1>>y2>>y3>>y4;
    long top = min(max(x1,x3),max(y3,y1));
    long bot = max(min(y1,y3),min(x1,x3));
    long left = max(min(x2,x4),min(y2,y4));
    long right = min(max(x2,x4),max(y2,y4));
    if(top>bot && left<right)
        cout<<(top-bot) * (right-left);
    else
        cout<<0;
}

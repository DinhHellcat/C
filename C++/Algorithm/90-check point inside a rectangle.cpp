#include<bits/stdc++.h>
using namespace std;
int main()
{
    long a1,b1,a2,b2,x,y;
    cin>>a1>>b1>>a2>>b2>>x>>y;
    long left = min(a1,a2);
    long bottom = min(b1,b2);
    long right = max(a1,a2);
    long top = max(b1,b2);
    if(left<x && x<right && bottom<y && y<top)
        cout<<"YES";
    else
        cout<<"NO";
}

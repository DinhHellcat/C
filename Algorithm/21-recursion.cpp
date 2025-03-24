#include<bits/stdc++.h>
using namespace std;
float sum(float x)
{
    if(x==1) return 0;
    return 1/x+sum(x-1);
}
int main()
{
    float n;
    cin>>n;
    cout<<fixed<<setprecision(4)<<sum(n);
}

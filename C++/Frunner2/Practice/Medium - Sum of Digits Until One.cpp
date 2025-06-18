#include<bits/stdc++.h>
using namespace std;
int transfer(int n)
{
    int x=0;
    string s = to_string(n);
    for(char c:s)
        x+=c-'0';
    return x;
}
int main()
{
    int n;
    cin>>n;
    while(n>=10)
    {
        n=transfer(n);
    }
    cout<<n;
}

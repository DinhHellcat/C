#include<bits/stdc++.h>
using namespace std;
int sum(string s)
{
    int x=0;
    for(char c:s)
        x+=c-'0';
    return x;
}
int main()
{
    string n;
    cin>>n;
    cout<<sum(n);
}

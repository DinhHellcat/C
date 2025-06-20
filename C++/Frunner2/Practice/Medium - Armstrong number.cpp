#include<bits/stdc++.h>
using namespace std;
int check(string s)
{
    int x=0;
    for(char c:s)
        x+=pow((c-'0'),3);
    return x;
}
int main()
{
    string s;
    cin>>s;
    int x=stoi(s,0,10);

    if(x==check(s))
        cout<<true;
    else
        cout<<false;
}

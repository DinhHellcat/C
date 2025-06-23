#include<bits/stdc++.h>
using namespace std;
string compile(string s)
{
    while(s.length()!=1)
    {
        int n=0;
        for(char c:s)
            n+=c-'0';
        s=to_string(n);
    }
    return s;
}
int main()
{
    string s;
    cin>>s;
    cout<<compile(s);
}

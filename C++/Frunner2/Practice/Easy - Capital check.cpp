#include<bits/stdc++.h>
using namespace std;

bool check(string& s, int n)
{
    if(n==0)
        return true;
    else if(n==1)
    {
        if(isupper(s[0]))
            return true;
    }
    else if(n==s.length())
        return true;

    return false;
}
int main()
{
    string s;
    cin>>s;
    int n=0;
    for(char c:s)
        if(isupper(c))
            n++;
    cout<<check(s,n);
}

#include<bits/stdc++.h>
using namespace std;

bool check(const string& s)
{
    for(int i=0;i<s.length()/2;i++)
        if(s[i]!=s[s.length()-i-1])
            return false;
    return true;
}
int main()
{
    string s,t;
    cin>>s;
    for(char i:s)
        if(isalnum(i))
            t.push_back(tolower(i));
    if(check(t))
        cout<<"true";
    else
        cout<<"false";
}

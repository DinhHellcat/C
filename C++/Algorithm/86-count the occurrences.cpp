#include<bits/stdc++.h>
using namespace std;
string removespace(const string& s)
{
    string r;
    for(char c : s)
        if(!isspace(c) && ((tolower(c)>='a'&&tolower(c)<='z') || (c>='0'&&c<='9')))
            r+=c;
    return r;
}
int main()
{
    multiset<char> n;
    set<char> x;
    string s;
    getline(cin,s);
    s=removespace(s);
    for(int i=0;i<s.length();i++)
    {
        n.insert(tolower(s[i]));
        x.insert(tolower(s[i]));
    }
    for(char c : x)
        cout<<c<<" "<<n.count(c)<<endl;
}

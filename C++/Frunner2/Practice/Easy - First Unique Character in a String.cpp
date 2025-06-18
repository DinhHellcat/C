#include<bits/stdc++.h>
using namespace std;
int progress(const string& s)
{
    vector<int> check(26,0);
    for(char c:s)
        check[c-'a']++;
    for(int i=0;i<s.length();i++)
        if(check[s[i]-'a']==1)
        {
            return i;
        }
    return -1;
}
int main()
{
    string s;
    cin>>s;
    cout<<progress(s);
}

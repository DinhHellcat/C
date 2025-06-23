#include<bits/stdc++.h>
using namespace std;
int check(string& s)
{
    vector<int> a(256,-1);
    int maxLength=0,first=0;
    for(int second=0;second<s.length();second++)
    {
        char c=s[second];
        if(a[c]>=first)
            first=a[c]+1;
        a[c]=second;
        maxLength=max(maxLength,second-first+1);
    }
    return maxLength;
}
int main()
{
    string s;
    getline(cin,s);
    cout<<check(s);
}

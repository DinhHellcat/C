#include<bits/stdc++.h>
using namespace std;

int main()
{
    string s,t;
    cin>>s>>t;
    bool check=true;
    sort(s.begin(),s.end());
    sort(t.begin(),t.end());
    for(int i=0;i<s.size();i++)
        if(s[i]!=t[i])
            check=false;
    cout<<check;
}

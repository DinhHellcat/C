#include<bits/stdc++.h>
using namespace std;

int main()
{
    int n;
    cin>>n;
    vector<string> s(n);
    for(int i=0;i<n;i++)
        cin>>s[i];
    sort(s.begin(),s.end(),[](const string& a,const string& b){return a+b>b+a;});
    string result="";
    for(string& i:s)
        result += i;
    if(!result.empty() && result[0]=='0')
        cout<<0;
    else
        cout<<result;
}

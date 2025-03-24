#include<bits/stdc++.h>
using namespace std;
int sum(string s)
{
    int x=0;
    for(int i=0;i<s.length();i++)
        x+=int(s[i]-48);
    return x;
}
int main()
{
    int n;
    cin>>n;
    string s;
    int a[i];
    cin.ignore();
    for(int i=0;i<n;i++)
    {
        getline(cin,s);
        a[i]=sum(s);
    }
    for(int i=0;i<n;i++)
    {
        cout<<a[i]<<endl;
    }
}

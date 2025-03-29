#include<bits/stdc++.h>
using namespace std;
int space(string s)
{
    int x=0,a=1,len=s.length();
    for(int i=0;i<len;i++)
    {
        if(s[i]==' ' && s[i+1]!=' ' && a==1)
        {
            x++;
            a=0;
        }
        if(s[i]!=' ' && s[i+1]==' ')
            a=1;
    }
    return x;
}
int main()
{
    int n,a[10001];
    cin>>n;
    string s;
    cin.ignore();
    for(int i=0;i<n;i++)
    {
        getline(cin,s);
        a[i]=space(s);
    }
    for(int i=0;i<n;i++)
        cout<<a[i]<<endl;
}

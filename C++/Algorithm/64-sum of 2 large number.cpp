#include<bits/stdc++.h>
using namespace std;
int main()
{
    string s1,s2;
    getline(cin,s1);
    getline(cin,s2);
    //cout<<s1<<endl<<s2<<endl;
    long long t=0,n,n1=s1.length(),n2=s2.length();
    if(s1<s2)
        s1.insert(0,n2-n1,'0');
    else
        s2.insert(0,n1-n2,'0');
    //cout<<s1<<endl<<s2<<endl;
    string s = s1;
    for(long long i=s.length()-1;i>=0;i--)
    {
        n=s1[i]+s2[i]-96+t;
        t=n/10;
        n%=10;
        s[i]=n+48;
    }
    if(t>0)
        s=char(t+48)+s;
    cout<<s;
}

#include<bits/stdc++.h>
using namespace std;
string sum(string s1, string s2)
{
    long long n1,n2,len,t=0;
    string s="";
    reverse(s1.begin(), s1.end());
    reverse(s2.begin(), s2.end());
    n1=s1.length();
    n2=s2.length();
    len=max(n1,n2);
    /*if(n1<n2)
        s1.insert(0,n2-n1,'0');
    else
        s2.insert(0,n1-n2,'0');*/
    if(n1<n2)
        for(int i=n1;i<n2;i++)
            s1.push_back('0');
    else
        for(int i=n2;i<n1;i++)
            s1.push_back('0');

    cout<<s1<<endl<<s2<<endl;
    for(int i=0;i<len;i++)
    {
        if(s1[i]+s2[i]-96<10)
        {
            s.push_back(char(s1[i]+s2[i]-48+t));
            t=0;
            //cout<<1<<" "<<t<<" "<<s1[i]<<" "<<s2[i]<<" "<<char(s1[i]+s2[i]-48+t)<<" "<<s<<endl;
        }
        else
        {
            s.push_back(char((s1[i]+s2[i]-48+t)-10));
            t=1;
            //cout<<2<<" "<<t<<" "<<s1[i]<<" "<<s2[i]<<" "<<char(s1[i]+s2[i]-48+t)<<" "<<s<<endl;
        }
        if(t==1 && i==len-1)
            s.push_back('1');
    }
    return s;
    c=a;
    for(long long i=a.length()-1;i>=0;i--)
        {
            tong=(a[i]-48)+(b[i]-48)+nho;
            nho=tong/10;
            tong=tong%10;
            c[i]=tong+48;
    }
    if(nho>0)
        { c=char(nho+48)+c; }
}
int main()
{
    string s1,s2;
    getline(cin,s1);
    getline(cin,s2);
    //s1="923";
    //s2="4321";
    //5244
    string s = sum(s1,s2);
    reverse(s.begin(),s.end());
    cout<<s;
}

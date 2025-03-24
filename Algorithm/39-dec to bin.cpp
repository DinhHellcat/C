#include<bits/stdc++.h>
using namespace std;
string dectobin(long long x)
{
    string s;
    while(x>0)
    {
        if(x%2==0)
            s+='0';
        else
            s+='1';
        x/=2;
    }
    return s;
}
int main()
{
    long long n;
    cin>>n;
    long long a[n];
    for(int i=0;i<n;i++)
        cin>>a[i];
    for(int i=0;i<n;i++)
    {
        string s = dectobin(a[i]);
        reverse(s.begin(), s.end());
        cout<<s<<endl;
    }
}

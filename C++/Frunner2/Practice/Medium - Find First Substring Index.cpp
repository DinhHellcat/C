#include<bits/stdc++.h>
using namespace std;
int main()
{
    string s, t;
    cin>>s>>t;
    for(int i=0;i<=s.length()-t.length();i++)
    {
        if(s[i]==t[0])
        {
            if(s.compare(i,t.length(),t)==0)
                {
                    cout<<i;
                    return 0;
                }
        }
    }
    cout<<-1;
}

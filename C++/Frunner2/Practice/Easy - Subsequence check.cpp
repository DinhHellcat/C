#include<bits/stdc++.h>
using namespace std;

int count(string s, string t)
{
    int n1=s.length(), n2=t.length(),j=0,n=0;
    for(int i=0;i<n2;i++)
        if(t[i]==s[j])
        {
            if(j==n1-1)
            {
                n=1;
                break;
            }
            j++;
        }
    if(n==1)
        return 1;
    return 0;
}
int main() {
    string s,t;
    getline(cin,s);
    getline(cin,t);
    cout<<count(s,t);
    return 0;
}

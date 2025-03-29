#include<bits/stdc++.h>
using namespace std;
int main()
{
    string s;
    getline(cin,s);
    int n = s.length();
    if(s[0]=='-')
        n--;
    cout<<n;
}

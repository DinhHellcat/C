#include<bits/stdc++.h>
using namespace std;
int main()
{
    string s;
    getline(cin,s);
    /*for(int i=0;i<s.length();i++)
        if(s[i]>='A' && s[i]<='Z')
            s[i]+=32;*/
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    cout<<s;
}

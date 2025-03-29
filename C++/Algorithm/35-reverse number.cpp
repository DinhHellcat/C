#include<bits/stdc++.h>
#include <string>
using namespace std;
int main(){
    string s;
    getline(cin, s);
    int l = s.length()-1;
    if(s[0]=='0' || s[0]=='-')
        return 0;
    for(int i=l;i>=0;i--)
    {
        if(s[i]!='0')
            break;
        l--;
    }
    for(int i=l;i>=0;i--)
        cout<<s[i];
}

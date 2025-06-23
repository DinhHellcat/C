#include<bits/stdc++.h>
using namespace std;
int UpperCase(string& s)
{
    int x=0;
    for(char c:s)
        if(c>='A' && c<='Z')
            x++;
    return x;
}
int LowerCase(string& s)
{
    int x=0;
    for(char c:s)
        if(c>='a' && c<='z')
            x++;
    return x;

}
int main()
{
    string s;
    getline(cin,s);
    int n;
    cin>>n;
    if(n==1)
        cout<<UpperCase(s);
    else
        cout<<LowerCase(s);
}

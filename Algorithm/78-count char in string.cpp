#include<bits/stdc++.h>
using namespace std;
string s;
int dem(char c)
{
    int n=0;
    for(int i=0;i<s.length();i++)
        if(tolower(s[i])==c)
            n++;
    return n;
}
int main()
{
    getline(cin,s);
    int n;
    cin>>n;
    char c;
    vector<int> a(n);
    for(int i=0;i<n;i++)
    {
        cin>>c;
        a[i]=dem(tolower(c));
    }
    for(int i=0;i<n;i++)
        cout<<a[i]<<endl;
}

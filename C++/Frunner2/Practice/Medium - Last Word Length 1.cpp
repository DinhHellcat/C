#include<bits/stdc++.h>
using namespace std;
int main()
{
    int i=0;
    string s,word;
    cin>>s;
    stringstream ss(s);
    vector<string> t;
    while(ss>>word)
    {
        t.push_back(word);
        i++;
    }
    cout<<t[i-1].length();
}

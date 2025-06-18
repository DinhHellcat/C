#include<bits/stdc++.h>
using namespace std;

int main()
{
    string s,word;
    getline(cin,s);
    stringstream ss(s);
    vector<string> t;
    while(ss>>word)
    {
        t.push_back(word);
    }
    cout<<t.size();
}

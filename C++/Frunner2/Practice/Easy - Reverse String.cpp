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
        reverse(word.begin(),word.end());
        t.push_back(word);
    }
    for(string i:t)
        cout<<i<<" ";
}

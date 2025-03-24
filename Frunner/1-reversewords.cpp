#include<bits/stdc++.h>
using namespace std;


int main() {
    int i=0;
    string s;
    getline(cin,s);
    stringstream ss(s);
    string word;
    vector<string> t;
    while(ss>>word)
    {
        t.push_back(word);
        i++;
    }
    for(int j=i-1;j>=0;j--)
        cout<<t[j]<<" ";
}

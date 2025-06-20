#include<bits/stdc++.h>
#include <variant>
using namespace std;

int main()
{
    /*using VarType = variant<char, long, double, string>;
    vector<VarType> a;*/

    vector<string> s;
    string t;
    while(cin>>t)
        s.push_back(t);
    for(string i:s)
        cout<<i<<endl;
}

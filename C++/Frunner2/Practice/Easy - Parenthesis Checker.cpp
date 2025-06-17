#include<bits/stdc++.h>
using namespace std;
bool check(string s)
{
    stack<char> a;
    for(char i : s)
    {
        if(i=='(' || i=='[' || i=='{')
            a.push(i);
        else
        {
            if(a.empty()) return false;
            else if((i==')' && a.top()=='(') || (i==']' && a.top()=='[') || (i=='}' && a.top()=='{'))
                a.pop();
            else
                return false;
        }
    }
    return true;
}
int main()
{
    string s;
    cin>>s;
    cout<<check(s);
}

#include<bits/stdc++.h>
using namespace std;
string capitalize(const string& name)
{
    string s;
    bool newW = true;
    for(char c : name)
    {
        if(isspace(c))
        {
            s+=c;
            newW=true;
        }
        else if(newW==true)
        {
            s+=toupper(c);
            newW=false;
        }
        else
            s+=tolower(c);
    }
    return s;
}

int main()
{
    int n;
    cin>>n;
    cin.ignore();
    vector<string> ss(n);
    for(int i=0;i<n;i++)
    {
        string s;
        getline(cin,s);
        ss[i]=capitalize(s);
    }
    for(const string &t : ss)
        cout<<t<<endl;
}

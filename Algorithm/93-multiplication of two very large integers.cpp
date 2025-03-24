#include<bits/stdc++.h>
using namespace std;
string sum(string a, string b)
{
    int len1 = a.size();
    int len2 = b.size();
    vector<int> result(len1 + len2,0);
    for(int i=len1-1;i>=0;i--)
        for(int j=len2-1;j>=0;j--)
        {
            int temp = (a[i]-'0')*(b[j]-'0');
            int mul  = temp + result[i+j+1];

            result[i+j+1] = mul%10;
            result[i+j]  += mul/10;
        }

    string s;
    bool firstnum = true;
    for(int i : result)
        if(!(i==0 && firstnum))
        {
            s+=to_string(i);
            firstnum = false;
        }

    return s.empty() ? "0" : s;
}
int main()
{
    string a,b;
    cin>>a>>b;
    cout<<sum(a,b);
}

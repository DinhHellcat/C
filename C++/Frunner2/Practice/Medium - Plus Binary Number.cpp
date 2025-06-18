#include<bits/stdc++.h>
using namespace std;

int main()
{
    string s="",s1,s2;
    cin>>s1>>s2;
    int dec1 = stoi(s1,0,2);
    int dec2 = stoi(s2,0,2);
    int sum=dec1+dec2;
    if(sum>0)
        while(sum>0)
        {
            s=char(sum%2+'0')+s;
            sum/=2;
        }
    else
        cout<<0;
    cout<<s;
}

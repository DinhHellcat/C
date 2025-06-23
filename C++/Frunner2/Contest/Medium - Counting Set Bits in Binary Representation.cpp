#include<bits/stdc++.h>
using namespace std;


int main() {
    int i=0,n;
    string s;
    cin>>n;
    while(n>0)
    {
        s=char(n%2+'0')+s;
        n/=2;
    }
    for(char c:s)
        if(c=='1')
            i++;
    cout<<i;
}

#include<bits/stdc++.h>
using namespace std;

bool check(const vector<vector<char>>& v)
{
    for(int i=0;i<9;i++)
    {
        bool num[10] = {false};
        for(int j=0;j<9;j++)
        {
            char x = v[i][j];
            if(x!='.')
            {
                if(num[x-'0']==true)
                    return 0;
                num[x-'0']=true;
            }
        }
    }
    for(int j=0;j<9;j++)
    {
        bool num[10] = {false};
        for(int i=0;i<9;i++)
        {
            char x = v[i][j];
            if(x!='.')
            {
                if(num[x-'0']==true)
                    return 0;
                num[x-'0']=true;
            }
        }
    }
    for(int i=0;i<9;i+=3)
        for(int j=0;j<9;j+=3)
        {
            bool num[10] = {false};
            for(int x=i;x<i+3;x++)
                for(int y=i;y<i+3;y++)
                {
                    char z = v[x][y];
                    if(z!='.')
                    {
                        if(num[z-'0']==true)
                            return 0;
                        num[z-'0']=true;
                    }
                }
        }
    return true;
}
int main()
{
    vector<vector<char>> t(9, vector<char>(9));
    for(int i=0;i<9;i++)
        for(int j=0;j<9;j++)
            cin>>t[i][j];
    cout<<check(t);
}

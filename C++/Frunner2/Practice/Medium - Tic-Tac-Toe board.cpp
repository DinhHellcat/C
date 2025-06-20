#include<bits/stdc++.h>
using namespace std;

char winner(vector<vector<char>>& a)
{
    for(int i=0;i<3;i++)
        if(a[i][0]!='-' && a[i][0]==a[i][1] && a[i][0]==a[i][2])
            return a[i][0];
    for(int i=0;i<3;i++)
        if(a[0][i]!='-' && a[0][i]==a[1][i] && a[0][i]==a[2][i])
            return a[0][i];
    if(a[1][1]!='-' && a[1][1]==a[0][0] && a[1][1]==a[2][2])
        return a[1][1];
    if(a[1][1]!='-' && a[1][1]==a[0][2] && a[1][1]==a[2][0])
        return a[1][1];
    return '-';
}
bool fullBoard(vector<vector<char>>& a)
{
    for(int i=0;i<3;i++)
        if(find(a[i].begin(),a[i].end(),'-')!=a[i].end())
            return false;
    return true;
}
int main()
{
    vector<vector<char>> a(3,vector<char>(3));
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            cin>>a[i][j];
    if(winner(a)=='X' || winner(a)=='O')
        cout<<winner(a);
    else
        if(fullBoard(a))
            cout<<"Draw";
        else
            cout<<"None";
}
/*int main()
{
    vector<vector<char>> a(3,vector<char> (3));
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            cin>>a[i][j];
    int check=2;
    char t;

    if(a[0][0]!='-' &&
       ((a[0][0] == a[1][1] && a[0][0] == a[2][2]) ||
       (a[0][0] == a[0][1] && a[0][0] == a[0][2]) ||
       (a[0][0] == a[1][0] && a[0][0] == a[2][0])))
       {
           check = 1;
           t=a[0][0];
       }
    else if
       (a[2][2]!='-' &&
        ((a[2][2] == a[2][1] && a[2][0] == a[2][2]) ||
        (a[0][2] == a[1][2] && a[0][2] == a[2][2])))
       {
            check = 1;
            t=a[2][2];
       }
    else if
        (a[1][1]!='-' &&
         ((a[1][1]==a[0][1] && a[1][1]==a[2][1]) ||
         (a[1][1]==a[1][0] && a[1][1]==a[1][2]) ||
         (a[1][1]==a[2][0] && a[1][1]==a[0][2])))
        {
            check = 1;
            t=a[1][1];
        }
    else
    {
        check = -1;
        for(int i=0;i<3;i++)
            if(find(a[i].begin(),a[i].end(),'-')!=a[i].end())
                check = 0;
    }
    switch(check)
    {
        case -1: cout<<"Draw";break;
        case 0: cout<<"None";break;
        case 1: cout<<t;break;
    }
}*/

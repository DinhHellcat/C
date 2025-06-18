#include<bits/stdc++.h>
using namespace std;

vector<int> compile(vector<vector<int>>& matrix)
{
    if(matrix.empty() || matrix[0].empty())
        return {};
    int m = matrix.size();
    int n = matrix[0].size();

    int top = 0;
    int bottom = m-1;
    int left = 0;
    int right = n-1;

    vector<int> temp;
    while(right>=left && bottom>=top)
    {
        for(int i=left;i<=right;i++)
            temp.push_back(matrix[top][i]);
        top++;

        for(int i=top;i<=bottom;i++)
            temp.push_back(matrix[i][right]);
        right--;

        if(top<=bottom)
        {
            for(int i=right;i>=left;i--)
                temp.push_back(matrix[bottom][i]);
            bottom--;
        }

        if(left<=right)
        {
            for(int i=bottom;i>=top;i--)
                temp.push_back(matrix[i][left]);
            left++;
        }
    }
    return temp;
}
int main()
{
    int m,n;
    cin>>m>>n;
    vector<vector<int>> matrix(m,vector<int>(n));
    for(int i=0;i<m;i++)
        for(int j=0;j<n;j++)
            cin>>matrix[i][j];
    vector<int> v = compile(matrix);
    for(int i:v) cout<<i<<" ";
}

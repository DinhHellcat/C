#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int longestIncreasingSubsequenceDP(const vector<int>& arr) {
    int n = arr.size();
    vector<int> dp(n, 1); // Initialize dp array with 1

    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (arr[i] > arr[j]) {
                dp[i] = max(dp[i], dp[j] + 1);
                cout<<i<<" "<<j<<" "<<"dp["<<i<<"]="<<dp[i]<<endl;
                for(int i=0;i<sizeof dp / sizeof(int);i++)
                    cout<<dp[i]<<" ";
                cout<<endl;
            }
        }
    }

    return *max_element(dp.begin(), dp.end()); // Return the maximum value in dp array
}

int main() {
    int N;
    cin >> N;
    vector<int> arr(N);
    for (int i = 0; i < N; i++) {
        cin >> arr[i];
    }

    cout << longestIncreasingSubsequenceDP(arr) << endl;

    return 0;
}

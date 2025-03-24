#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int longestIncreasingSubsequenceBinarySearch(const vector<int>& arr) {
    vector<int> lis;

    for (int num : arr) {
        auto pos = lower_bound(lis.begin(), lis.end(), num);
        if (pos == lis.end()) {
            lis.push_back(num); // Append to the LIS sequence
        } else {
            *pos = num; // Replace the element at the position
        }
    }

    return lis.size(); // The size of lis is the length of the longest increasing subsequence
}

int main() {
    int N;
    cin >> N;
    vector<int> arr(N);
    for (int i = 0; i < N; i++) {
        cin >> arr[i];
    }

    cout << longestIncreasingSubsequenceBinarySearch(arr) << endl;

    return 0;
}
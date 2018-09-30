#include <iostream>
#include <algorithm>
using namespace std;

int *wagers;

bool binarySearch(int value, int n) {
    int left = 0, right = n - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        int midValue = wagers[mid];
        if (midValue == value) {
            return true;
        } else if (midValue > value) {
            right = mid - 1;
        } else { //midValue < value
            left = mid + 1;
        }
    }
    return false;
}

int main() {
    int n;
    while (1) {
        cin >> n;
        if (n == 0) break;
        wagers = new int[n];
        for (int i = 0; i < n; i++) cin >> wagers[i];
        sort(wagers, wagers + n);
        for (int i = n - 1; i >= 0; i--) {
            for (int a = 0; a < n; a++){
                if (a == i) continue;
                for (int b = 0; b < n; b++) {
                    if (b == a || b == i) continue;
                    int needValue = wagers[i] - wagers[a] - wagers[b];
                    if (needValue == wagers[i] || needValue == wagers[a] || needValue == wagers[b]) continue;
                    if (binarySearch(needValue, n)) {
                        cout << wagers[i] << endl;
                        goto Free;
                    }
                }
            }
        }
        cout << "no solution" << endl;
        Free: {
            free(wagers);
            wagers = NULL;
        }
    }
	return 0;
}

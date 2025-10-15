#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef long long ll;

ll max_total_value(ll* nums, int n, ll k) {
    if (n == 0) {
        return 0;
    }

    ll max_val = LLONG_MIN;
    ll min_val = LLONG_MAX;

    for (int i = 0; i < n; i++) {
        if (nums[i] > max_val) {
            max_val = nums[i];
        }
        if (nums[i] < min_val) {
            min_val = nums[i];
        }
    }

    ll max_single_value = max_val - min_val;

    ll result = k * max_single_value;

    return result;
}

int main() {
    int n;
    ll k;

    n = 3;
    k = 2;
    ll nums[] = {1, 3, 2};
    
    ll max_total = max_total_value(nums, n, k);
    
    printf("Input Array: [");
    for (int i = 0; i < n; i++) {
        printf("%lld%s", nums[i], (i == n - 1) ? "" : ", ");
    }
    printf("]\n");
    printf("k: %lld\n", k);
    printf("Maximum Possible Total Value: %lld\n", max_total);

    return 0;
}

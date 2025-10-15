// code written in c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

typedef uint64_t ll;
const ll M = 998244353;

ll power(ll base, ll exp) {
    ll res = 1;
    base %= M;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % M;
        base = (base * base) % M;
        exp /= 2;
    }
    return res;
}

ll modInverse(ll a) {
    return power(a, M - 2);
}

ll power_high_exp(ll base, ll exp) {
    ll res = 1;
    base %= M;
    return power(base, exp);
}

ll geometric_sum(ll r, ll n) {
    if (n == 0) return 0;
    if (n == 1) return 1 % M;
    r %= M;
    if (r == 1) return n % M;

    ll r_n = power_high_exp(r, n);
    ll numerator = (r_n - 1 + M) % M;
    ll denominator = (r - 1 + M) % M;
    return (numerator * modInverse(denominator)) % M;
}

ll calculate_C_d_full(ll d) {
    ll P_d = power(10, d);
    ll N_d = 1ULL << (d - 1);
    
    ll C_d = 0;

    ll S_1_sum = geometric_sum(P_d, N_d);
    ll S_1 = (power(10, d - 1) * S_1_sum) % M;
    C_d = (C_d + S_1) % M;

    for (ll i = 2; i <= d; ++i) {
        ll P = 1ULL << (d - i);
        ll T_P = geometric_sum(P_d, P);
        
        ll num_blocks = 1ULL << (i - 2);
        
        ll k_start_first_block = (1ULL << (d - 1)) + P;
        ll R_d_minus_1 = (1ULL << d) - 1;
        ll power_exponent_start = (R_d_minus_1 - k_start_first_block) * d;

        ll first_term = power_high_exp(10, power_exponent_start);
        
        ll common_ratio_exponent = -(2 * P * d);
        ll common_ratio = power_high_exp(10, (M - 1) + common_ratio_exponent);

        ll S_i_geometric_sum = geometric_sum(common_ratio, num_blocks);
        
        ll S_i_inner = (first_term * S_i_geometric_sum) % M;
        S_i_inner = (S_i_inner * T_P) % M;

        ll S_i = (power(10, d - i) * S_i_inner) % M;
        C_d = (C_d + S_i) % M;
    }
    return C_d;
}

ll calculate_C_d_partial(ll d, ll n) {
    ll L_d = 1ULL << (d - 1);
    ll N_d = n - L_d + 1;
    ll P_d = power(10, d);
    
    ll C_d = 0;

    ll S_1_sum = geometric_sum(P_d, N_d);
    ll S_1 = (power(10, d - 1) * S_1_sum) % M;
    C_d = (C_d + S_1) % M;

    for (ll i = 2; i <= d; ++i) {
        ll j = i - 1;
        ll P = 1ULL << (d - i);
        ll period = 1ULL << (d - j);
        
        ll T_P = geometric_sum(P_d, P);
        
        ll num_full_periods = N_d / period;
        ll remainder = N_d % period;

        ll S_i_inner = 0;
        
        ll common_ratio_exponent = -period * d;
        ll common_ratio = power_high_exp(10, (M - 1) + common_ratio_exponent);


        if (num_full_periods > 0) {
            ll k_start_0 = L_d + P; 
            ll power_exponent_start_0 = (n - k_start_0) * d;

            ll first_term = power_high_exp(10, power_exponent_start_0);
            
            ll S_i_geometric_sum = geometric_sum(common_ratio, num_full_periods);
            
            ll full_periods_contrib = (first_term * S_i_geometric_sum) % M;
            full_periods_contrib = (full_periods_contrib * T_P) % M;
            S_i_inner = (S_i_inner + full_periods_contrib) % M;
        }

        if (remainder > P) {
            ll m_start_rem_block = num_full_periods * period + P;
            
            ll k_start_rem = L_d + m_start_rem_block;
            ll power_exponent_start_rem = (n - k_start_rem) * d;
            
            ll block_len_rem = remainder - P;
            if (block_len_rem > P) block_len_rem = P;

            ll T_rem = geometric_sum(P_d, block_len_rem);

            ll rem_block_contrib = power_high_exp(10, power_exponent_start_rem);
            rem_block_contrib = (rem_block_contrib * T_rem) % M;
            S_i_inner = (S_i_inner + rem_block_contrib) % M;
        }

        ll S_i = (power(10, d - i) * S_i_inner) % M;
        C_d = (C_d + S_i) % M;
    }
    return C_d;
}

void solve() {
    int t_count;
    if (scanf("%d", &t_count) != 1) return;

    for (int i = 0; i < t_count; ++i) {
        ll n;
        if (scanf("%" PRIu64, &n) != 1) return;

        ll R = 0;
        ll d_max = (ll)floor(log2(n)) + 1;

        for (ll d = 1; d < d_max; ++d) {
            ll N_d = 1ULL << (d - 1);
            ll C_d = calculate_C_d_full(d);

            ll power_factor = power_high_exp(10, d * N_d);
            R = (R * power_factor) % M;
            R = (R + C_d) % M;
        }

        ll d = d_max;
        ll L_d = 1ULL << (d - 1);
        ll N_d = n - L_d + 1;
        ll C_d = calculate_C_d_partial(d, n);

        ll power_factor = power_high_exp(10, d * N_d);
        R = (R * power_factor) % M;
        R = (R + C_d) % M;
        
        printf("%" PRIu64 "\n", R);
    }
}

int main() {
    solve();
    return 0;
}


a^n=a^(n mod phi(m)) (mod m) if gcd(a,m)=1

int phi(int n) {
    int ans = n;
    for (int i = 2; 1LL * i * i <= n; i++) {
        if (n % i == 0) {
            while (n % i == 0) n /= i;
            ans -= ans / i;
        }
    }
    if (n > 1) ans -= ans / n;
    return ans;
}
int euler_power(int a, long long n, int m) {
    return binpow(a, n % phi(m), m);
}
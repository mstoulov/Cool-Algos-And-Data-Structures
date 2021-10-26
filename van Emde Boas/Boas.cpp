#include <bits/stdc++.h>
#define int long long
using namespace std;

template <int K>
class Van_De_Boas {
 public:
    Van_De_Boas() : K1(K1_vec[K]), K2(K2_vec[K]) {
        ch.reserve(1 << K1);
        for (int i = 0; i < 1 << K1; ++i) ch.push_back(Van_De_Boas<K2_vec[K]>());
        aux = new Van_De_Boas<K3_vec[K]>;
        mn = mx = -1;
    }
    int high(size_t n) { return n << 64 - (K1 + K2) >> 64 - K1; }
    int low(size_t n) { return n << 64 - K2 >> 64 - K2; }
    int merge(size_t n1, size_t n2) { return n1 << K2 | n2; }
    bool empty() { return mn == -1; }
    int min() { return mn; }
    int max() { return mx; }
    void insert(int n) {
        if (mn == -1) {
            mn = mx = n;
        } else if (mn == mx) {
            if (n < mn) mn = n;
            else mx = n;
        } else {
            if (n < mn) swap(n, mn);
            if (n > mx) swap(n, mx);
            int h = high(n);
            if (ch[h].empty()) aux->insert(h);
            ch[h].insert(low(n));
        }
    }
    void remove(int n) {
        if (mn == -1) return;
        if (mn == mx) {
            mn = mx = -1;
            return;
        }
        if (mn == n) {
            mn = aux->empty() ? mx : merge(aux->min(), ch[aux->min()].min());
            n = mn;
        } else if (mx == n) {
            mx = aux->empty() ? mn : merge(aux->max(), ch[aux->max()].max());
            n = mx;
        }
        int h = high(n);
        ch[h].remove(low(n));
        if (ch[h].empty()) aux->remove(h);
    }
    int next(int n) {
        if (mn == -1 || n >= mx) return -1;
        if (mn > n) return mn;
        int h = high(n), l = low(n);
        if (ch[h].max() > l) return merge(h, ch[h].next(l));
        int next_high = aux->next(h);
        if (next_high == -1) return mx;
        return merge(next_high, ch[next_high].min());
    }
    int prev(int n) {
        if (mn == -1 || n <= mn) return -1;
        if (mx < n) return mx;
        int h = high(n), l = low(n);
        if (ch[h].min() < l) return merge(h, ch[h].prev(l));
        int prev_high = aux->prev(h);
        if (prev_high == -1) return mn;
        return merge(prev_high, ch[prev_high].max());
    }
 private:
    constexpr static int const K2_vec_tmp[37] {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24};
    constexpr static int const K1_vec_tmp[37] {0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    constexpr static int const K3_vec_tmp[37] {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 8, 9, 10, 11, 12, 6, 6, 6, 6, 6, 6, 7, 8, 9, 10, 11, 12};
    
    constexpr static const int* K1_vec = &K1_vec_tmp[0];
    constexpr static const int* K2_vec = &K2_vec_tmp[0];
    constexpr static const int* K3_vec = &K3_vec_tmp[0];
    
    int mn, mx;
    vector<Van_De_Boas<K2_vec[K]>> ch;
    Van_De_Boas<K3_vec[K]>* aux;
    uint8_t K1, K2;
};

template <>
class Van_De_Boas<6> {
 public:
    Van_De_Boas() : bt(0) {}
    bool empty() { return !bt; }
    int min() { return bt ? __builtin_ctzll(bt) : -1; }
    int max() { return bt ? 64 - 1 - __builtin_clzll(bt) : -1; }
    void insert(int n) { bt |= 1ull << n; }
    void remove(int n) { bt &= ~(1ull << n); }
    int next(int n) {
        if (n == 63) return -1;
        return bt >> n + 1 ? __builtin_ctzll(bt >> n + 1) + n + 1 : -1;
    }
    int prev(int n) {
        if (n == 63) return -1;
        return bt << 64 - n ? n - __builtin_clzll(bt << 64 - n) - 1 : -1;
    }
 private:
    size_t bt;
};

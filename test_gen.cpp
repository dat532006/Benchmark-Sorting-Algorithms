/**
 * test_gen.cpp — Adversarial Test Case Generator
 * Môn: [Tên môn học] — [Học kỳ, Năm]
 * Sinh viên: [Họ tên] — MSSV: [MSSV]
 *
 * Biên dịch (C++23):
 *   g++ -std=c++2b -O2 -o test_gen.o test_gen.cpp
 *
 * Sử dụng:
 *   ./test_gen.o <int|strlexi|strlenlexi> <1|2|3|4|5>
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <cstdlib>
#include <cstdint>
#include <numeric>

// ============================================================
// GIỚI HẠN THEO ĐỀ BÀI
// ============================================================

namespace limits {
    // Bài A - Integer Sort: n<=10^5, signed 32-bit, TL=10s
    constexpr int INT_N       = 100000;
    constexpr int32_t I_MIN   = INT32_MIN;
    constexpr int32_t I_MAX   = INT32_MAX;

    // Bài B - Lexicographic Sort: n<=10^5, len in [10,100], TL=10s
    constexpr int SLEXI_N     = 100000;

    // Bài C - Length-aware Lexi Sort: n<=10^4, len in [10,100], TL=1s
    constexpr int SLLEXI_N    = 10000;

    // Chung cho B và C
    constexpr int LEN_MIN     = 10;
    constexpr int LEN_MAX     = 100;
}

// ============================================================
// TIỆN ÍCH
// ============================================================

static std::mt19937_64 rng(42);

long long ri(long long lo, long long hi) {
    return std::uniform_int_distribution<long long>(lo, hi)(rng);
}

char rc() { return static_cast<char>('a' + ri(0, 25)); }

// ============================================================
// BÀI A: INTEGER SORT (n<=10^5, signed 32-bit, TL=10s)
// Format: dòng 1 = n, n dòng tiếp mỗi dòng 1 số
// ============================================================

void gen_int(int t) {
    int n = limits::INT_N;
    std::cout << n << "\n";

    switch (t) {
    case 1: // Giảm dần → worst-case Insertion/Bubble Sort O(n²)
        for (int i = n; i >= 1; --i)
            std::cout << i << "\n";
        break;

    case 2: // Tăng dần → worst-case Quicksort pivot đầu/cuối
        for (int i = 1; i <= n; ++i)
            std::cout << i << "\n";
        break;

    case 3: // Tất cả giống nhau → worst-case QS không 3-way partition
        for (int i = 0; i < n; ++i)
            std::cout << 42 << "\n";
        break;

    case 4: // INT_MAX xen kẽ INT_MIN → kiểm tra overflow
        for (int i = 0; i < n; ++i)
            std::cout << (i % 2 == 0 ? limits::I_MAX : limits::I_MIN) << "\n";
        break;

    case 5: { // Organ-pipe → đánh bại median-of-three
        std::vector<int> a(n);
        int v = 1;
        for (int i = 0; i < n; i += 2) a[i] = v++;
        for (int i = n - 1 - (n % 2 == 0 ? 1 : 0); i >= 0; i -= 2) a[i] = v++;
        for (int i = 0; i < n; ++i)
            std::cout << a[i] << "\n";
        break;
    }
    }
}

// ============================================================
// BÀI B: LEXICOGRAPHIC SORT (n<=10^5, len∈[10,100], TL=10s)
// Format: dòng 1 = n, n dòng tiếp mỗi dòng 1 chuỗi
// ============================================================

void gen_strlexi(int t) {
    int n = limits::SLEXI_N;
    int L = limits::LEN_MAX; // luôn dùng 100 để max compare cost

    std::cout << n << "\n";

    switch (t) {
    case 1: // Prefix 99 'a', khác ký tự cuối → compare phải duyệt 99 chars
        for (int i = 0; i < n; ++i) {
            std::string s(L, 'a');
            s[L - 1] = static_cast<char>('a' + (i % 26));
            std::cout << s << "\n";
        }
        break;

    case 2: // Tất cả chuỗi giống hệt → compare luôn = → max work
        for (int i = 0; i < n; ++i) {
            std::string s(L, 'a');
            std::cout << s << "\n";
        }
        break;

    case 3: { // Reverse sorted → worst cho insertion sort + compare cost cao
        rng.seed(33333);
        std::vector<std::string> v(n);
        for (int i = 0; i < n; ++i) {
            std::string s(L, 'a');
            int x = i;
            for (int j = L-1; j >= L-6 && x > 0; --j, x /= 26)
                s[j] = static_cast<char>('a' + (x % 26));
            v[i] = s;
        }
        std::sort(v.begin(), v.end());
        std::reverse(v.begin(), v.end());
        for (auto &s : v) std::cout << s << "\n";
        break;
    }

    case 4: { // Prefix 95 'a', suffix 5 ngẫu nhiên → duyệt 95% mới phân biệt
        rng.seed(44444);
        for (int i = 0; i < n; ++i) {
            std::string s(L, 'a');
            for (int j = 95; j < L; ++j) s[j] = rc();
            std::cout << s << "\n";
        }
        break;
    }

    case 5: // Xen kẽ "aaa...aaa" và "aaa...aab" → compare duyệt hết
        for (int i = 0; i < n; ++i) {
            std::string s(L, 'a');
            if (i % 2 == 0) s[L - 1] = 'b';
            std::cout << s << "\n";
        }
        break;
    }
}

// ============================================================
// BÀI C: LENGTH-AWARE LEXI SORT (n<=10^4, len∈[10,100], TL=1s!)
// Sort: (length ASC, rồi lexi ASC nếu cùng length)
// Bài này TL rất chặt! O(n²*L) = 10^4 * 10^4 * 100 = 10^10 → TLE
// ============================================================

void gen_strlenlexi(int t) {
    int n = limits::SLLEXI_N;
    int L = limits::LEN_MAX;

    std::cout << n << "\n";

    switch (t) {
    case 1: // Cùng dài 100, prefix 99 giống, khác ký tự cuối
            // → cùng len → fallback sang lexi → duyệt 99 chars mỗi compare
        for (int i = 0; i < n; ++i) {
            std::string s(L, 'a');
            s[L - 1] = static_cast<char>('a' + (i % 26));
            std::cout << s << "\n";
        }
        break;

    case 2: // Cùng dài 100, tất cả giống hệt → compare = luôn → max work
        for (int i = 0; i < n; ++i) {
            std::string s(L, 'a');
            std::cout << s << "\n";
        }
        break;

    case 3: { // Cùng dài 100, reverse lexi order → worst insertion sort
        rng.seed(77777);
        std::vector<std::string> v(n);
        for (int i = 0; i < n; ++i) {
            std::string s(L, 'a');
            int x = i;
            for (int j = L-1; j >= L-5 && x > 0; --j, x /= 26)
                s[j] = static_cast<char>('a' + (x % 26));
            v[i] = s;
        }
        std::sort(v.begin(), v.end());
        std::reverse(v.begin(), v.end());
        for (auto &s : v) std::cout << s << "\n";
        break;
    }

    case 4: // Nửa dài 100, nửa dài 10, xen kẽ → swap cost O(len)
        for (int i = 0; i < n; ++i) {
            int thisL = (i % 2 == 0) ? 100 : 10;
            std::string s(thisL, 'a');
            s[thisL - 1] = static_cast<char>('a' + (i % 26));
            std::cout << s << "\n";
        }
        break;

    case 5: { // Cùng dài 100, prefix 98 'a', suffix 2 ngẫu nhiên
        rng.seed(99999);
        for (int i = 0; i < n; ++i) {
            std::string s(L, 'a');
            s[L - 2] = rc();
            s[L - 1] = rc();
            std::cout << s << "\n";
        }
        break;
    }
    }
}

// ============================================================
// MAIN
// ============================================================

int main(int argc, char* argv[]) {
    std::ios::sync_with_stdio(false);
    std::cout.tie(nullptr);

    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <int|strlexi|strlenlexi> <1|2|3|4|5>\n";
        return 1;
    }

    std::string p = argv[1];
    int t = std::atoi(argv[2]);

    if (t < 1 || t > 5) {
        std::cerr << "Error: test_id must be 1-5\n";
        return 1;
    }

    if      (p == "int")        gen_int(t);
    else if (p == "strlexi")    gen_strlexi(t);
    else if (p == "strlenlexi") gen_strlenlexi(t);
    else {
        std::cerr << "Error: unknown problem '" << p << "'\n";
        return 1;
    }
    return 0;
}

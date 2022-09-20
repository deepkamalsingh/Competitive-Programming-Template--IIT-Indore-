#include <ext/pb_ds/assoc_container.hpp>
using namespace __gnu_pbds;

mt19937 rng((uint32_t)chrono::steady_clock::now().time_since_epoch().count());
const long double PI = acos((long double)-1);


struct custom_hash { /// use most bits rather than just the lowest ones
  const uint64_t C = (long long)(2e18 * PI) + 71; // large odd number
  const int RANDOM = rng();
  long long operator()(long long x) const { /// https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
    return __builtin_bswap64((x^RANDOM)*C); }
};


template<class K,class V> using hash_table = gp_hash_table<K,V,custom_hash>;
template<class K,class V> V get(hash_table<K,V>& u, K x) {
  auto it = u.find(x); return it == end(u) ? 0 : it->second; 
}

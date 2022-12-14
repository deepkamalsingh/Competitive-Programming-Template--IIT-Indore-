// #include "mod int"

auto random_address = [] { char *p = new char; delete p; return uint64_t(p); };
mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count() * (random_address() | 1));
 
// P = 2^32 - 13337 is a safe prime: both P and (P - 1) / 2 are prime.
extern const unsigned HASH_P = unsigned(-13337); // hash prime
using hash_int = _m_uint<HASH_P>;
using hash_t = uint64_t;

const uint64_t HASH_P2 = uint64_t(HASH_P) * HASH_P;// square of hash prime
const int HASH_COUNT = 2;
 
// Avoid multiplication bases near 0 or P - 1.
uniform_int_distribution<unsigned> MULT_DIST(unsigned(0.1 * HASH_P), unsigned(0.9 * HASH_P));
const hash_int HASH_MULT[] = {MULT_DIST(rng), MULT_DIST(rng)}; // our bases
const hash_int HASH_INV[] = {1 / HASH_MULT[0], 1 / HASH_MULT[1]};
 
vector<hash_int> hash_pow[] = {{1}, {1}};
 
const int INF = int(1e9) + 5;
 
template<typename T_string = string>
struct string_hash {
	static const bool BUILD_REVERSE = false;
 
  static uint64_t hash(const T_string &str) {
  	uint64_t result = 0;
 
    for(int h = 0; h < HASH_COUNT; h++) {
      uint64_t value = 1;
 			for(const auto &x : str){
 				value = (uint64_t(HASH_MULT[h]) * value + x) % HASH_P;
 			}
    
    	result += value << (32 * h);
   	}
 
  	return result;
  }
 
  T_string str;
  vector<hash_int> _prefix[HASH_COUNT];
  vector<hash_int> _inv_prefix[HASH_COUNT];
 
  string_hash() {
  	build({});
  }
 
  string_hash(const T_string &_str) {
  	build(_str);
  }
 
  int length() const {
  	return int(_prefix[0].size()) - 1;
  }
 
  template<typename T_char>
  void add_char(const T_char &c) {
  	str.push_back(c);
 
    for(int h = 0; h < HASH_COUNT; h++) {
    	_prefix[h].push_back(HASH_MULT[h] * _prefix[h].back() + c);
 
      if(hash_pow[h].size() < _prefix[h].size()){
      	hash_pow[h].push_back(HASH_MULT[h] * hash_pow[h].back());
 			}
 			
    	if(BUILD_REVERSE){
    		_inv_prefix[h].push_back((_inv_prefix[h].back() + c) * HASH_INV[h]);
    	}
                
    }
  }
 
  void pop_char() {
  	str.pop_back();
 
    for(int h = 0; h < HASH_COUNT; h++) {
    	_prefix[h].pop_back();
 
      if(BUILD_REVERSE) {
      	_inv_prefix[h].pop_back();
      }
                
    }
  }
 
  void build(const T_string &_str) {
  	str = {};
    str.reserve(_str.size());
 
    for(int h = 0; h < HASH_COUNT; h++) {
    	hash_pow[h].reserve(_str.size() + 1);
      _prefix[h] = {0};
      _prefix[h].reserve(_str.size() + 1);
 
      if(BUILD_REVERSE) {
      	_inv_prefix[h] = {0};
        _inv_prefix[h].reserve(_str.size() + 1);
      }
  	}
 
    for(auto &c : _str)	add_char(c);
  }
 
  uint64_t _single_hash(int h, int start, int end) const {
  	// Convert everything to `uint64_t` for speed. Note: we add hash_pow[length] to fix strings that start with 0.
    uint64_t power = uint64_t(hash_pow[h][end - start]);
  	return (power + uint64_t(_prefix[h][end]) + HASH_P2 - uint64_t(_prefix[h][start]) * power) % HASH_P;
  }
 
  uint64_t substring_hash(int start, int end) const {
  	assert(0 <= start && start <= end && end <= length());
    return _single_hash(0, start, end) + (HASH_COUNT > 1 ? _single_hash(1, start, end) << 32 : 0);
  }
 
  uint64_t complete_hash() const {
    return substring_hash(0, length());
  }
 
  uint64_t _reverse_single_hash(int h, int start, int end) const {
    // Convert everything to `uint64_t` for speed. Note: we add hash_pow[length] to fix strings that start with 0.
    uint64_t power = uint64_t(hash_pow[h][end - start]);
    return (power + uint64_t(_inv_prefix[h][end]) * power + HASH_P - uint64_t(_inv_prefix[h][start])) % HASH_P;
  }
 
  uint64_t reverse_substring_hash(int start, int end) const {
    assert(0 <= start && start <= end && end <= length());
    return _reverse_single_hash(0, start, end) + (HASH_COUNT > 1 ? _reverse_single_hash(1, start, end) << 32 : 0);
  }
 
  uint64_t reverse_complete_hash() const {
    return reverse_substring_hash(0, length());
  }
 
  bool equal(int start1, int start2, int length) const {
  	return substring_hash(start1, start1 + length) == substring_hash(start2, start2 + length);
  }
 
  bool is_palindrome(int start, int end) const {
    return substring_hash(start, end) == reverse_substring_hash(start, end);
  }
 
  int compare(int start1, int start2, int max_length = INF) const;
};
 
uint64_t concat_hashes(uint64_t hash1, uint64_t hash2, int len2) {
  uint64_t hash1_low = hash1 & unsigned(-1);
  uint64_t hash2_low = hash2 & unsigned(-1);
  uint64_t power = uint64_t(hash_pow[0][len2]);
  uint64_t combined = (hash1_low * power + hash2_low + HASH_P - power) % HASH_P;
 
  if (HASH_COUNT > 1) {
    hash1 >>= 32;
    hash2 >>= 32;
    power = uint64_t(hash_pow[1][len2]);
    combined += (hash1 * power + hash2 + HASH_P - power) % HASH_P << 32;
  }
 
  return combined;
}
 
template<typename T_string>
int first_mismatch(const string_hash<T_string> &hash1, int start1,
                   const string_hash<T_string> &hash2, int start2, int max_length = INF) {
	max_length = min({max_length, hash1.length() - start1, hash2.length() - start2});
 
  static const int FIRST = 5;
  int first = min(max_length, FIRST);
 
  for(int i = 0; i < first; i++){
  	if(hash1.str[start1 + i] != hash2.str[start2 + i]){
  		return i;
  	}
  }
        
 
  if(hash1.substring_hash(start1, start1 + max_length) == hash2.substring_hash(start2, start2 + max_length)){
  	return max_length;
 	}
 	
  static const int MANUAL = 15;
  int low = first, high = max_length - 1;
 
  while(high - low > MANUAL) {
  	int mid = (low + high + 1) / 2;
		if(hash1.substring_hash(start1, start1 + mid) == hash2.substring_hash(start2, start2 + mid)){
			low = mid;
		}else{
      high = mid - 1;
    }
  }
 
  for(int i = low; i < high; i++) {
    if(hash1.str[start1 + i] != hash2.str[start2 + i]){
    	return i;
    }
  }
 
  return high;
}
 
template<typename T_string>
int hash_compare(const string_hash<T_string> &hash1, int start1,
                 const string_hash<T_string> &hash2, int start2, int max_length = INF) {
  int mismatch = first_mismatch(hash1, start1, hash2, start2, max_length);
  int length1 = min(hash1.length() - start1, max_length);
  int length2 = min(hash2.length() - start2, max_length);
 
  if(mismatch == min(length1, length2)){
  	return length1 == length2 ? 0 : (length1 < length2 ? -1 : +1);
  }
        
 
  if(hash1.str[start1 + mismatch] == hash2.str[start2 + mismatch]){
  	return 0;
  }
        
 
  return hash1.str[start1 + mismatch] < hash2.str[start2 + mismatch] ? -1 : +1;
}
 
template<typename T_string>
int string_hash<T_string>::compare(int start1, int start2, int max_length) const {
  return hash_compare(*this, start1, *this, start2, max_length);
}









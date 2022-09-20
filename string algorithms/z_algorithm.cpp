// Also known as "extended KMP"
template<typename T>
vector<int> z_algorithm(const T &pattern){
  // Z[i] = for the suffix [i, n) of pattern, the longest prefix that is also a prefix of pattern.
  int n = int(pattern.size());
  vector<int> Z(n, 0);
  Z[0] = n;
  // Index whose Z value forms the right border or this can also be called as left border.
  int loc = 1;
 
  for(int i = 1; i < n; i++){
    if(i < loc + Z[loc]){
      Z[i] = min(Z[i - loc], loc + Z[loc] - i);
    }
    for(; i + Z[i] < n && pattern[Z[i]] == pattern[i + Z[i]]; ++Z[i]);
    // Find the location with rightmost border.
    if(i + Z[i] > loc + Z[loc]){
      loc = i;
    }
  }
  return Z;
}



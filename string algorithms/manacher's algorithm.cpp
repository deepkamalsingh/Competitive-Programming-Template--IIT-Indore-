// Computes the radius of the longest palindrome centered at each element in O(n) total.
// Actual palindrome length is `2 * r[i] + 1`.
template<typename T_string>
vector<int> odd_manacher(const T_string &pattern){
  int n = pattern.size();
  vector<int> r(n, 0);
  int loc = 0;
 
  for(int i = 1; i < n; i++){
    if(i <= loc + r[loc]){
      r[i] = min(r[loc - (i - loc)], loc + r[loc] - i);
    }
          
    for(;i - r[i] > 0 && i + r[i] < n - 1 && pattern[i - r[i] - 1] == pattern[i + r[i] + 1]; ++r[i]);
    
    if(i + r[i] > loc + r[loc]){
      loc = i;
    }
  }
  return r;
}
 
// Returns a vector `lengths` of size `2 * n + 1` where `lengths[i]` indicates a palindrome starting at
// `(i - lengths[i])/2` and ending at `(i + lengths[i])/2 - 1`.
template<typename T_string>
vector<int> combined_manacher(const T_string &pattern){
  int n = pattern.size();
  T_string extended(2 * n + 1, 0);
  for(int i = 0; i < n; i++){
    extended[2 * i + 1] = pattern[i];
  }
  return odd_manacher(extended);
}
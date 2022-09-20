void sum_over_subset(vector<int>& a, bool reverse = false){
  int n = 31 - __builtin_clz(a.size());
  for(int i = 0; i < n; ++i)
    for(int j = 0; j < (1 << n); ++j)
      if(j >> i & 1)
        a[j] += (reverse ? -1 : +1) * a[j^(1<<i)];
}

void sum_over_superset(vector<int>& a, bool reverse = false){
  int n = 31 - __builtin_clz(a.size());
  for(int i = 0; i < n; ++i)
    for(int j = 0; j < (1 << n); ++j)
      if(j >> i & 1)
        a[j^(1<<i)] += (reverse ? -1 : +1) * a[j];
}

// a[i] * b[j] contributes to result[i&j]
vector<int> and_conv(vector<int> a, vector<int> b){ 
  sum_over_superset(a);
  sum_over_superset(b);
  for(int i = 0; i < int(a.size()); ++i)
    a[i] *= b[i];
  sum_over_superset(a,true);
  return a;
}

// a[i] * b[j] contributes to result[i|j]
vector<int> or_conv(vector<int> a, vector<int> b){ 
  sum_over_subset(a);
  sum_over_subset(b);
  for(int i = 0; i < int(a.size()); ++i)
    a[i] *= b[i];
  sum_over_subset(a,true);
  return a;
}
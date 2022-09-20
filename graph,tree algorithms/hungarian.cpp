// calculate minimum weighted bipartite matching.
template<class T>
vector<int> hungarian(const vector<vector<T>> &c){

  vector<T> le, ri;
  vector<int> perm;

  int n = int(c.size()), m = int(c[0].size());
  assert(n <= m);
  le = vector<T>(n, T(0));
  ri = vector<T>(m, T(0));
  perm = vector<int>(n);
  vector<int> to_r(n, -1), to_l(m, -1);

  for(int s = 0; s < n; s++){

    vector<char> l_u(n), r_u(m);
    l_u[s] = true;
    vector<int> tr(m, -1), min_l(m, s);
    vector<T> min_cost(m);

    for(int j = 0; j < m; j++)
      min_cost[j] = c[s][j] + le[s] + ri[j];

    while(true){

      int r = -1;
      T d = numeric_limits<T>::max();

      for(int j = 0; j < m; j++){
        if(!r_u[j] && min_cost[j] < d)
          r = j, d = min_cost[j];
      }

      for(int i = 0; i < n; i++)
        if(l_u[i])  le[i] -= d;

      for(int j = 0; j < m; j++)
        if(r_u[j]) ri[j] += d;
        else  min_cost[j] -= d;
      
      tr[r] = min_l[r];
      int l = to_l[r];
      if(l == -1){
        while(r != -1){
          int nl = tr[r], nr = to_r[nl];
          to_l[r] = nl;
          to_r[nl] = r;
          r = nr;
        }
        break;
      }
      l_u[l] = r_u[r] = true;
      for(int j = 0; j < m; j++){
        T cost = c[l][j] + le[l] + ri[j];
        if(cost < min_cost[j]){
          min_l[j] = l;
          min_cost[j] = cost;
        }
      }
    }

  }

  return perm = to_r;
}
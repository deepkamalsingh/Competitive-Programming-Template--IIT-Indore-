// #include "strongly connected components"

pair<bool,vector<bool>> two_sat(int n,const vector<pair<int,int>> &conditions){

  vector<vector<edge>> g(2 * n);

  auto negation = [&](int i){ return i >= n ? i - n : i + n; };

  for(auto [u,v] : conditions){
    g[negation(u)].push_back({v});
    g[negation(v)].push_back({u});
  }

  auto [comp,topo] = strongly_cc(g);

  for(int i = 0; i < n; ++i)
    if(comp[i] == comp[i + n])
      return {false,{}};

  vector<bool> ret(n);
  vector<int> wh(2 * n,-1);

  for(int i = 0; i < int(topo.size()); ++i) wh[topo[i]] = i;

  for(int i = 0; i < n; ++i){
    if(wh[comp[i]] < wh[comp[i + n]])
      ret[i] = false;
    else
      ret[i] = true;
  }

  return {true,ret};
}
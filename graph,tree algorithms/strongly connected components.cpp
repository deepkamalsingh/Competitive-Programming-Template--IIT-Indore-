struct edge{
  int to;
};

// returns component identifier, and topological sort of them.
pair<vector<int>,vector<int>> strongly_cc(const vector<vector<edge>> &g){
  int timer = 0, n = g.size();
  vector<int> disc(n), comp(n, -1), topo;
  stack<int> s;

  auto dfs = [&](auto self,int x) -> int {
    int low = disc[x] = ++timer;
    s.push(x);
    for(auto [y] : g[x])
      if(comp[y] == -1)
        low = min(low,disc[y]?:self(self,y));
    if(low == disc[x]){
      topo.push_back(x);
      for(int y = -1; y != x;){
        comp[y = s.top()] = x;
        s.pop();
      }
    }
    return low;
  };

  for(int i = 0; i < n; ++i)
    if(!disc[i])
      dfs(dfs,i);

  reverse(topo.begin(), topo.end());

  return {comp, topo};
}
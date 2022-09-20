struct edge{
  int to;
};

// returns {#bcc components, block tree}.
pair<int,vector<vector<int>>> bi_cc(const vector<vector<edge>> &g){

  int n = g.size();

  vector<int> depth(n,-1), low(n), comp;
  vector<vector<int>> block_tree(n);
  stack<int> s;
  int z = 0, f = -1;

  auto dfs = [&](auto self,int node,int parent) -> void {
    low[node] = depth[node];

    for(auto [child] : g[node])
      if(child != parent){
        if(depth[child] < 0){
          s.push(f); int here = f; --f;
          s.push(child); s.push(node);
          depth[child] = depth[node] + 1;
          self(self,child,node);
          if(low[child] >= depth[node]){
          
            while(comp.empty() || comp.back() != here)
              comp.push_back(s.top()), s.pop();

            sort(all(comp));
            comp.erase(unique(all(comp)),comp.end());
            
            for(auto i : comp) if(i >= 0) block_tree[i].pb(n+z);

            comp = {};
            z++;
          }
          low[node] = min(low[node],low[child]);
        }else if(depth[child] < depth[node]){
          s.push(child); s.push(node);
          low[node] = min(low[node],depth[child]);
        }
      }
    
    return;
  };

  for(int i = 0; i < n; ++i)
    if(depth[i] < 0){
      depth[i] = 0;
      dfs(dfs,i,-1);
    }

  block_tree.resize(z + n);
  fr(i,0,n){
    for(auto j : block_tree[i]){
      assert(j >= n);
      block_tree[j].pb(i);
    }
  } 

  return {z,block_tree};
}
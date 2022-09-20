struct edge{
  int to, id;
};

// returns {cut vertices, edge id's that form a biconnected component}.
pair<vector<bool>,vector<vector<int>>> bi_cc(const vector<vector<edge>> &g){

  int n = g.size();

  vector<bool> cut_vertex(n, false);
  vector<int> depth(n,-1), low(n), comp;
  vector<vector<int>> bcc;
  stack<int> s;

  auto dfs = [&](auto self,int node,int parent_id) -> void {
    low[node] = depth[node];
    
    bool ok = false;
    int children = 0;

    for(auto [child,id] : g[node])
      if(id != parent_id){
        if(depth[child] < 0){
          s.push(id);
          depth[child] = depth[node] + 1;
          self(self,child,id);
          if(low[child] >= depth[node]){
            ok = true;
            // no idea how this part works ?
            while(comp.empty() || comp.back() != id)
              comp.push_back(s.top()), s.pop();
            bcc.push_back(comp), comp = {};
          }
          low[node] = min(low[node],low[child]);
          children++;
        }else if(depth[child] < depth[node]){
          s.push(id);
          low[node] = min(low[node],depth[child]);
        }
      }
    
    if(parent_id >= 0){
      cut_vertex[node] = ok;
    }else{
      // check if root is cut vertex.
      if(children > 1)
        cut_vertex[node] = true;
    }
    
    return;
  };

  for(int i = 0; i < n; ++i)
    if(depth[i] < 0){
      depth[i] = 0;
      dfs(dfs,i,-1);
    }

  return {cut_vertex,bcc};
}
// #include "hash map"

const int INF = int(1e9) + 5;

struct edge {
  int node;
  edge(int _node = -INF) : node(_node) {}
};

struct CD {
	int N;
  vector<vector<edge>> adj;
  vector<int> depth;
  vector<int> subtree_size;
  // parent of a node in centroid tree.
  vector<int> centroid_parent;
  vector<int> node_list;
  // gives the distance of each node to its descendants in centroid tree.
	vector<hash_table<int, int>> centroid_dist; // from root to node.


  void init(int _N) {
    N = _N;
    adj.assign(N, {});
    depth.resize(N);
    subtree_size.resize(N);
    centroid_parent.assign(N, -1);
    centroid_dist.resize(N);
  }


  void add_edge(int u, int v) {
    assert(u != v);
    adj[u].emplace_back(v);
    adj[v].emplace_back(u);
  }

  // Erasing edges is O(number of nodes remaining) which is fine within our decomposition.
	void erase_edge(int from, int to) {
		for(edge &e : adj[from]) {
      if(e.node == to) {
        swap(e, adj[from].back());
        adj[from].pop_back();
        return;
      }
    }
    assert(false);
  }

  int dfs(int node, int parent = -1, int subroot = -1) {
  	if(parent < 0) {
    	subroot = node;
      node_list.clear();
    }

    depth[node] = parent < 0 ? 0 : depth[parent] + 1;
    subtree_size[node] = 1;
    node_list.push_back(node);

    for(edge &e : adj[node]) {
    	if(e.node != parent) {
      	subtree_size[node] += dfs(e.node, node, parent < 0 ? e.node : subroot);
      }
    }

    return subtree_size[node];
  }

  int centroid(int root) {
  	int n = dfs(root);
    bool found;

    // Repeatedly move to the subtree that is at least half of the tree, if such a subtree exists.
    do {
      found = false;

      for(edge &e : adj[root]){
        if(subtree_size[e.node] < subtree_size[root] && 2 * subtree_size[e.node] >= n) {
          root = e.node;
          found = true;
          break;
      	}
      }
    } while(found);

    return root;
  }

	// centroid parent of root of centroid tree is -1
  void solve(int root) {
  	root = centroid(root);
    dfs(root);

    for(int node : node_list){
      if(node != root){
         centroid_parent[node] = root;
			}
      centroid_dist[root][node] = depth[node];
    }

  	for(edge &e : adj[root]){
    	erase_edge(e.node, root);
    }

  	// Recurse after solving root, so that edge erasures don't cause incorrect results.
    for(edge &e : adj[root]){
      solve(e.node);
    }
  }
};
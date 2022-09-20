// #include "segment tree"
 
template<bool VALS_IN_EDGES> 
struct HLD { 
	int N;
	int timer;
	vector<vector<int>> adj;
	vector<int> par, root, depth, sz,  pos;
	vector<int> rpos; // rpos not used, but could be useful
	
	// segment_tree<S,op,e> tree; // segment tree 
	
	void init(int _N){
		N = _N;
		adj.assign(N,{});
		par.assign(N,-1);
		root.assign(N,-1);
		depth.assign(N,-1);
		sz.assign(N,-1);
		pos.assign(N,-1);
		tree.init(N);
	}
	
	
	void ae(int x, int y){ 
		adj[x].push_back(y), adj[y].push_back(x); 
	}
	
	void dfs_sz(int x){ 
		sz[x] = 1; 
		for(auto& y : adj[x]){
			par[y] = x; depth[y] = depth[x] + 1;
			adj[y].erase(find(adj[y].begin(),adj[y].end(),x)); // remove parent from adj list
			dfs_sz(y); 
			sz[x] += sz[y];
			if(sz[y] > sz[adj[x][0]]) 
				swap(y,adj[x][0]); // store the heavy child at first vertex
		}
	}
	
	void dfs_hld(int x){
		pos[x] = timer++; rpos.push_back(x);
		for(auto& y : adj[x]){
			root[y] = (y == adj[x][0] ? root[x] : y);
			dfs_hld(y); 
		}
	}
	
	
	
	void gen(int R = 0){ 
		par[R] = depth[R] = timer = 0; 
		dfs_sz(R); 
		root[R] = R;
		dfs_hld(R); 
	}
	
	int lca(int x, int y){
		for(; root[x] != root[y]; y = par[root[y]])
			if(depth[root[x]] > depth[root[y]]) 
				swap(x,y);

		return depth[x] < depth[y] ? x : y;
	}
	
	int dist(int x, int y){ // # edges on path
	 	return depth[x] + depth[y] - 2 * depth[lca(x,y)]; 
	}
	
	
	
	void process_path(int x, int y, auto op){
		for(; root[x] != root[y]; y = par[root[y]]){
			if(depth[root[x]] > depth[root[y]]) 
				swap(x,y);
			op(pos[root[y]],pos[y]); 
		}
		if(depth[x] > depth[y]) 
			swap(x,y);
		op(pos[x]+VALS_IN_EDGES,pos[y]); 
	}
 
	void modify_path(int x, int y, int v){ 
		process_path(x,y,[this,&v](int l, int r){ 
			// tree.upd(l,v); 
		});
	}
 
	int query_path(int x, int y){ 
		int res = 0; 
		process_path(x,y,[this,&res](int l, int r){ 
			// res = max(res,tree.query(l,r)); 
		});
		return res; 
	}
 
 	/*
 	 * this is for range update.
	void modify_subtree(int x, int v){ 
		tree.upd(pos[x] + VALS_IN_EDGES, pos[x] + sz[x] - 1, v); 
	}
	
	*/
};
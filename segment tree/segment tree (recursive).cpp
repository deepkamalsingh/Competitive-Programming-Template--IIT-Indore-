template<class T,T (*comb)(T,T),T id> 
struct segment_tree {
  int size;
  vector<T> tree;

  segment_tree(int n = -1){
    if(n >= 0)
      init(n);
  }

  void init(int n){
    size = 1;
    while(size < n) size <<= 1;
    tree.resize(size << 1);
    build(0, 0, size);
  }
  
  void build(int x, int lx, int rx){
    if(rx - lx == 1){
      tree[x] = id;
      return;
    }

    int mid = (lx + rx)/2;
    build(2 * x + 1, lx, mid);
    build(2 * x + 2, mid, rx);
    tree[x] = comb(tree[2 * x + 1], tree[2 * x + 2]);
  }
  
  
  void update(int pos, T value, int x, int lx, int rx){
    if(rx - lx == 1){
      tree[x] = value;  
      return;
    }

    int mid = (rx + lx)/2 ;
    if(pos < mid)
      update(pos, value, 2 * x + 1, lx, mid);
    else
      update(pos, value, 2 * x + 2, mid, rx);
    
    tree[x] = comb(tree[2 * x + 1], tree[2 * x + 2]);
  }
  
  // update value at pos p.
  void update(int pos, T value){
    update(pos, value, 0, 0, size);
  }
  
  // query on [lx,rx).
  T query(int l, int r, int x, int lx, int rx){
    if(lx >= r || l >= rx)
      return id;
    
    if(lx >= l && rx <= r)
      return tree[x] ;
    
    int mid = (rx + lx)/2;
    return  comb(query(l, r, 2 * x + 1, lx, mid), query(l, r, 2 * x + 2, mid, rx));
  }
  
  T query(int l, int r){
    return query(l, r, 0, 0, size);
  }
};


// https://atcoder.github.io/ac-library/master/document_en/lazysegtree.html 
// It is the data structure for the pair of a monoid (S, \cdot: S \times S \to S, e \in S)(S,⋅:S×S→S,e∈S) and a set FF of S \to SS→S mappings that satisfies the following properties.

// FF contains the identity map \mathrm{id}id, where the identity map is the map that satisfies \mathrm{id}(x) = xid(x)=x for all x \in Sx∈S.
// FF is closed under composition, i.e., f \circ g \in Ff∘g∈F holds for all f, g \in Ff,g∈F.
// f(x \cdot y) = f(x) \cdot f(y)f(x⋅y)=f(x)⋅f(y) holds for all f \in Ff∈F and x, y \in Sx,y∈S.
// Given an array SS of length NN, it processes the following queries in O(\log N)O(logN) time (see Appendix for further details).

// Acting the map f\in Ff∈F (cf. x = f(x)x=f(x)) on all the elements of an interval
// Calculating the product of the elements of an interval
// For simplicity, in this document, we assume that the oracles op, e, mapping, composition, and id work in constant time. If these oracles work in O(T)O(T) time, each time complexity appear in this document is multipled by O(T)O(T).

// The following should be defined.

// The type S of the monoid
// The binary operation S op(S a, S b)
// The function S e() that returns ee
// The type F of the map
// The function S mapping(F f, S x) that returns f(x)f(x)
// The function F composition(F f, F g) that returns f \circ gf∘g
// The function F id() that returns \mathrm{id}id

template< class S,
          S (*op)(S, S),
          S (*e)(),
          class F,
          S (*mapping)(F, S),
          F (*composition)(F, F),
          F (*id)()>
struct lazy_segtree{

public:
  lazy_segtree() : lazy_segtree(0) {}
  lazy_segtree(int n) : lazy_segtree(vector<S>(n, e())) {}
  lazy_segtree(const vector<S>& v) : _n(int(v.size())){
    log = 0;
    while(1 << log < _n) log++;
    size = 1 << log;
    d = vector<S>(2 * size, e());
    lz = vector<F>(size, id());
    for(int i = 0; i < _n; i++) d[size + i] = v[i];
    for(int i = size - 1; i >= 1; i--) update(i);
  }

  void set(int p, S x){
    assert(0 <= p && p < _n);
    p += size;
    for(int i = log; i >= 1; i--) push(p >> i);
    d[p] = x;
    for(int i = 1; i <= log; i++) update(p >> i);
  }

  S get(int p){
    assert(0 <= p && p < _n);
    p += size;
    for(int i = log; i >= 1; i--) push(p >> i);
    return d[p];
  }

  S prod(int l, int r){
    assert(0 <= l && l <= r && r <= _n);
    if (l == r) return e();

    l += size;
    r += size;

    for(int i = log; i >= 1; i--){
      if(((l>>i)<<i) != l) push(l >> i);
      if(((r>>i)<<i) != r) push(r >> i);
    }

    S sml = e(), smr = e();
    for(; l < r; l >>= 1, r >>= 1){
      if(l&1) sml = op(sml, d[l++]);
      if(r&1) smr = op(d[--r], smr);
    }

    return op(sml, smr);
  }

  void apply(int p, F f){
    assert(0 <= p && p < _n);
    p += size;
    for(int i = log; i >= 1; i--) push(p >> i);
    d[p] = mapping(f, d[p]);
    for(int i = 1; i <= log; i++) update(p >> i);
  }

  void apply(int l, int r, F f){
    assert(0 <= l && l <= r && r <= _n);
    if(l == r) 
      return;

    l += size;
    r += size;

    for(int i = log; i >= 1; i--){
      if(((l>>i)<<i) != l) push(l >> i);
      if(((r>>i)<<i) != r) push((r - 1) >> i);
    }

    {
        int l2 = l, r2 = r;
        for(; l < r; l >>= 1, r >>= 1){
          if(l&1) all_apply(l++, f);
          if(r&1) all_apply(--r, f);
        }
        l = l2;
        r = r2;
    }

    for(int i = 1; i <= log; i++){
      if(((l>>i)<<i) != l) update(l >> i);
      if(((r>>i)<<i) != r) update((r - 1) >> i);
    }
  }

private:
  int _n, size, log;
  vector<S> d;
  vector<F> lz;

  void update(int k){ d[k] = op(d[2 * k], d[2 * k + 1]); }

  void all_apply(int k, F f){
    d[k] = mapping(f, d[k]);
    if(k < size) lz[k] = composition(f, lz[k]);
  }

  void push(int k){
    all_apply(2 * k, lz[k]);
    all_apply(2 * k + 1, lz[k]);
    lz[k] = id();
  }
};
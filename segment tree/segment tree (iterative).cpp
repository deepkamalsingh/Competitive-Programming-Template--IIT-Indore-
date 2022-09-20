// S is monoid
// https://en.wikipedia.org/wiki/Monoid
template<class S,S (*op)(S,S),S (*e)()>
struct segment_tree{

public:
  segment_tree() : segment_tree(0) {}
  segment_tree(int n) : segment_tree(vector<S> (n,e())) {}
  segment_tree(const vector<S>& v){
    _n = int(v.size());
    log = 0;
    while(1 << log < _n) log++;
    size = 1 << log;
    d = vector<S>(2 * size, e());
    for(int i = 0; i < _n; ++i) d[size + i] = v[i];
    for(int i = size - 1; i >= 1; --i)
      update(i);
  }

  // set a[p] = x.
  void set(int p,S x){
    assert(0 <= p && p < _n);
    d[p += size] = x;
    for(int i = 1; i <= log; ++i) update(p >> i);
  }

  // return a[p].
  S get(int p){
    assert(0 <= p && p < _n);
    return d[p + size];
  }

  // op(a[l], a[l+1], ..., a[r-1]), if l == r return e()
  S prod(int l,int r){
    assert(0 <= l && l <= r && r <= _n);
    S sml = e(), smr = e();
    for(l += size, r += size; l < r; l >>= 1, r >>= 1){
      if(l&1) sml = op(sml, d[l++]);
      if(r&1) smr = op(d[--r],smr);
    }
    return op(sml,smr);
  }

  // return an index r such that.
  // r = l or f(op(a[l], a[l + 1], ..., a[r - 1])) = true
  // r = n or f(op(a[l], a[l + 1], ..., a[r])) = false
  // If f is monotone, this is the maximum r that satisfies f(op(a[l], a[l + 1], ..., a[r - 1])) = true.
  template<bool (*f)(S)> int max_right(int l){
    return max_right(l, [](S x){ return f(x); });
  }

  template<class F> int max_right(int l,F f){
    assert(0 <= l && l <= _n);
    assert(f(e()));
    if(l == _n) return _n;
    l += size;
    S sm = e();
    do{
      if(l%2 == 0) l >>= 1;
      if(!f(op(sm,d[l]))){
        while(l < size){
          l <<= 1;
          if(f(op(sm,d[l]))){
            sm = op(sm,d[l]);
            l++;
          }
        }
        return l - size;
      }
      sm = op(sm,d[l]);
      l++;
    }while((l&-l) != l);
    return _n;
  }

  // return an index l such that
  // l = r or f(op(a[l], a[l + 1], ..., a[r - 1])) = true
  // l = 0 or f(op(a[l - 1], a[l], ..., a[r - 1])) = false
  // if f is monotone, this is the minimum l that satisfies f(op(a[l], a[l + 1], ..., a[r - 1])) = true.
  template<bool (*f)(S)> int min_left(int r){
    return min_left(r, [](S x){ return f(x); });
  }

  template<class F> int min_left(int r,F f){
    assert(0 <= r && r <= _n);
    assert(f(e()));
    if(r == 0) return 0;
    r += size;
    S sm = e();
    do{
      r--;
      if(r > 1 && r%2 == 1) r >>= 1;
      if(!f(op(d[r],sm))){
        while(r < size){
          r = r << 1 | 1;
          if(f(op(d[r],sm))){
            sm = op(d[r],sm);
            r--;
          }
        }
        return r + 1 - size;
      }
      sm = op(d[r],sm);
    }while((r&-r) != r);
    return _n;
  }


private:
  int _n, log, size;
  vector<S> d;

  void update(int k){
    d[k] = op(d[k << 1], d[k << 1 | 1]);
  }

};

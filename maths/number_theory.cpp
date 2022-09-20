// divide a by b rounded up
template<typename T>
T cdiv(T a,T b){ 
  return a/b + ((a^b) > 0 && a%b); 
} 

// divide a by b rounded down
template<typename T>
T fdiv(T a,T b){ 
  return a/b - ((a^b) < 0 && a%b); 
}

template<typename T> 
T gcd(T a,T b,T& x,T& y){
  if(b == 0){
    x = 1, y = 0;
    return a;
  }
  T x1, y1;
  T d = gcd(b, a % b, x1, y1);
  x = y1;
  y = x1 - y1 * (a / b);
  return d;
}

// returns x0 and y0 s.t. a * x0 + b * y0 = c if it exists.
template<typename T> 
bool find_any_solution(T a,T b,T c,T &x0,T &y0){
  T g = gcd(abs(a), abs(b), x0, y0);
  if(c%g)  return false;
  x0 *= c/g;
  y0 *= c/g;
  if(a < 0) x0 *= -1;
  if(b < 0) y0 *= -1;
  return true;
}

// returns the number of integers between 1 and n  inclusive, which are coprime to n.
template<typename T>
T totient(T n){
  T result = n;
  for(T i = 2; i * i <= n; ++i){
    if(n%i == 0){
      while(n%i == 0)
        n /= i;
      result -= result/i;
    }
  }
  if(n > 1)
    result -= result / n;
  return result;
}

vector<int> totient_all(int n){
  vector<int> phi(n + 1);
  for(int i = 0; i <= n; i++)
    phi[i] = i;

  for(int i = 2; i <= n; i++){
    if(phi[i] == i){
      for(int j = i; j <= n; j += i)
        phi[j] -= phi[j] / i;
    }
  }

  return phi;
}

template<typename T>
T safe_mod(T x,T m){
  x %= m;
  if(x < 0) x += m;
  return x;
}

template<typename T>
pair<T,T> inv_gcd(T a,T b){
  a = safe_mod(a, b);
  if(a == 0) 
    return {b, 0};

  T s = b, t = a;
  T m0 = 0, m1 = 1;
    
  while(t){
    T u = s / t;
    s -= t * u, m0 -= m1 * u;
    T tmp = s;
    s = t, t = tmp, tmp = m0, m0 = m1, m1 = tmp;
  }
 
  if(m0 < 0) 
    m0 += b / s;
  return {s, m0};
}

// Returns a number that is a1 mod m1 and a2 mod m2. Assumes m1 and m2 are relatively prime.
template<typename T>
T crt(T a1, T m1, T a2, T m2){
  T k = (a2 - a1) % m2 * inv_mod(m1, m2) % m2;
  T result = a1 + k * m1;
 
  if(result < 0)  result += m1 * m2;
 
  assert(0 <= result && result < m1 * m2);
  assert(result % m1 == a1 && result % m2 == a2);
  return result;
}
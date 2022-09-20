void ntt (vector<int> & a, bool invert) {
	int n = (int) a.size();
	for (int i=1, j=0; i<n; ++i) {
		int bit = n >> 1;
		for (; j>=bit; bit>>=1)
			j -= bit;
		j += bit;
		if (i < j)
			swap (a[i], a[j]);
	}
	for (int len=2; len<=n; len<<=1) {
		int wlen = invert ? root_1 : root;
		for (int i=len; i<root_pw; i<<=1)
			wlen = int (wlen * 1ll * wlen % mod);
		for (int i=0; i<n; i+=len) {
			int w = 1;
			for (int j=0; j<len/2; ++j) {
				int u = a[i+j],  v = int (a[i+j+len/2] * 1ll * w % mod);
				a[i+j] = u+v < mod ? u+v : u+v-mod;
				a[i+j+len/2] = u-v >= 0 ? u-v : u-v+mod;
				w = int (w * 1ll * wlen % mod);
			}
		}
	}
	if (invert){
		int nrev = inverse(n);
		for (int i=0; i<n; ++i)
			a[i] = int (a[i] * 1ll * nrev % mod);
	}
}
vi multiply(vi a,vi b){
	int n1=a.size();
	int n2=b.size();
	int gg=n1+n2-1;
	int m=1;
	while(m<gg)m<<=1;
	a.resize(m);
	b.resize(m);
	ntt(a,false);
	ntt(b,false);
	vi c(m);
	for(int i=0;i<m;i++){
		c[i]=(1LL*a[i]*b[i])%mod;
	}
	ntt(c,true);
	c.resize(gg);
	return c;
}
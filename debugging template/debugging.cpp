namespace OUT{
  template<class T>
  void __p(const T& a){ cout<<a; }
  template<class T, class F>
  void __p(const pair<T, F>& a){ cout<<"{"; __p(a.first); cout<<","; __p(a.second); cout<<"}\n"; }
  template<class T, size_t N>
  void __p(const array<T,N>& a){ cout<<"{"; for(int i=0;i<N;++i)__p(a[i]),cout<<",}\n"[i+1==N]; }
  template<class T>
  void __p(const vector<T>& a){
    cout<<"{";for(auto it=a.begin();it<a.end();it++)__p(*it),cout<<",}\n"[it+1==a.end()]; }
  template<class T, class ...Arg>
  void __p(T a1, Arg ...a){__p(a1); __p(a...); }
  template<class Arg1>
  void __f(const char *s, Arg1 &&arg1){ cout<<s<<" : "; __p(arg1); cout<<endl; }
  template<class Arg1, class ... Args>
  void __f(const char *ss, Arg1 &&arg1, Args &&... args){
    int b=0,i=0; do{ if(ss[i]=='(') b++; if(ss[i]==')') b--; i++;}while(!(ss[i]==','&&b==0));
    const char *comma=ss+i; cout.write(ss,comma-ss)<<" : ";__p(arg1);cout<<" | ";__f(comma+1,args...);}
  #define trace(...) cout<<"Line:"<<__LINE__<<"  ", __f(#__VA_ARGS__, __VA_ARGS__)
}
using namespace OUT;

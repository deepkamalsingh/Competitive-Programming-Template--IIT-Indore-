template<int ALPHABET>
struct prefix_tree {
	
	struct trie_node {
  	int link[ALPHABET];
    int what;
    int depth;
 
    trie_node(){
      memset(link, -1, sizeof(link));
    }
	};
	
	vector<trie_node> trie;
	
	prefix_tree(){
		trie.emplace_back();
		trie[0].depth = 0;
		trie[0].what = -1;
	}
	
	void create_node(int parent, int c){
    trie.emplace_back();
    trie.back().what = c;
    trie.back().depth = trie[parent].depth + 1;
	}
	
	int get_or_create_node(int current, int c){
    if(trie[current].link[c] < 0){
      trie[current].link[c] = trie.size();
      create_node(current, c);
    }
    return trie[current].link[c];
	}
 
	int insert_str(const string &str){
		int current = 0;
    for(char c : str){
      current = get_or_create_node(current, c - 'a');
 		}
    return current;
	}
};
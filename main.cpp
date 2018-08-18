#include <iostream>
#include <vector>
using namespace std;

class IterativeSegTree {
	int n;
	unsigned long sz;
	vector<int> arr;
	vector<int> tree;
	// TODO: Pass _combiner_ as std::function to the constructor
	//       This will keep the design generic.
	unsigned long get_size(int n) {
		// get closest power of 2 >= n
		unsigned long lim = 1;
		while(lim < n) lim <<= 1;
		return lim;
	}

	void set_leaf_nodes() {
		for(int i = n ; i < 2 * n ; i++) {
			tree[i] = arr[i - n];
		}
	}

	void construct_tree() {
		for(int i = n - 1 ; i >= 1 ; i--) {
			tree[i] = combine(tree[2*i], tree[2*i + 1]);
		}
	}

	int combine(int x, int y) {
		return min(x, y);
	}

public:
	explicit IterativeSegTree(int len_a, vector<int> a) {
		n = len_a;
		sz = get_size(n);
		arr = a; // copy array
		tree.resize((unsigned long)2 * sz); // 2n elements in tree
		set_leaf_nodes();
		construct_tree();
	}

	void print_tree() {
		for(int i = 1 ; i < tree.size() ; i++) {
			cout << i << ": " << tree[i] << "\n";
		}
	}
};

int main() {
	int n; cin >> n;
	vector<int> a(n);
	for(int i = 0 ; i < n ; i++) {
		cin >> a[i];
	}
	IterativeSegTree *segTree = new IterativeSegTree(n, a);
	segTree->print_tree();
}


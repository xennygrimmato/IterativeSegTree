#include <iostream>
#include <vector>
using namespace std;

class IterativeSegTree {
	int n;
	unsigned long sz;
	vector<int> arr; // redundant copy of first array from which segtree was created
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

	int identity() {
		return 0x3f3f3f3f; // identity element for min function is INF
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

	void update(int idx, int new_val) {
		idx += n;
		tree[idx] = new_val;
		while (idx > 1) {
			idx /= 2;
			tree[idx] = combine(tree[2*idx], tree[2*idx + 1]);
		}
	}

	// range query on [left, right)
	int query(int left, int right) {
		left += n, right += n;
		int ret = identity();
		while (left < right) {
			if (left & 1) {
				ret = combine(ret, tree[left]);
				left++;
			}
			if(right & 1) {
				right--; // first exclude right, because our spec is [left, right)
				ret = combine(ret, tree[right]);
			}
			left >>= 1;
			right >>= 1;
		}
		return ret;
	}

	void print_tree() {
		for(int i = 1 ; i < tree.size() ; i++) {
			cout << i << ": " << tree[i] << "\n";
		}
	}
};

void test1() {
	/*
	 * array of size 2^k
	 * 8
		 1 5 3 7 3 6 5 7
	 */
	int n = 8;
	vector<int> a = {1, 5, 3, 7, 3, 6, 5, 7};
	IterativeSegTree *segTree = new IterativeSegTree(n, a);
	segTree->print_tree();
	cout << segTree->query(3, 8); // ans should be 3
	delete segTree;
}

void test2() {
	/*
	 * array of even size != 2^k
	 * 6
		 4 3 8 1 6 7
	 */
	int n = 6;
	vector<int> a = {4, 3, 8, 1, 6, 7};
	IterativeSegTree *segTree = new IterativeSegTree(n, a);
	segTree->print_tree();
	cout << segTree->query(0, 5); // ans should be 1
	delete segTree;
}

int main() {
	test1();
	test2();
}


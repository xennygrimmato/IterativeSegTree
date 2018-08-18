//
// Created by Vaibhav Tulsyan on 18/08/18.
//

// Problem link: http://codeforces.com/contest/1023/problem/D
// AC Submission link: http://codeforces.com/contest/1023/submission/41747576

#include <iostream>
#include <vector>
#include <set>
#include <cstring>
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
	cout << segTree->query(0, 5); // ans should be 3
	delete segTree;
}

//int main() {
//	test1();
//	test2();
//}

const int N = 5e5;
int n, q;
int mn[N], mx[N];
set<int> zero_idx; // maintain indexes of all zeros
vector<int> a;

IterativeSegTree *seg;

void get_first_last_occurrence() {
	for(int i = 0 ; i < n ; i++) {
		cin >> a[i];
		mn[a[i]] = min(mn[a[i]], i);
		mx[a[i]] = max(mx[a[i]], i);
		if(!a[i]) zero_idx.insert(i);
		else seg->update(i, a[i]);
	}
}

bool validate_segments() {
	// For value Y, no element X < Y should occur
	// between the leftmost and rightmost occurrence
	// of Y
	for(int i = q ; i > 0 ; i--){
		if(mn[i] > mx[i]) continue;
		if(seg->query(mn[i], mx[i]) < i){
			return false;
		}
	}
	return true;
}

bool validate_for_q() {
	if(mn[q] > mx[q] and zero_idx.empty()) return false;
	return true;
}

int main() {
	cin >> n >> q;
	a.resize(n, 0x3f3f3f3f);
	seg = new IterativeSegTree(n, a);
	memset(mn, 0x3f, sizeof(mn));
	get_first_last_occurrence();
	bool ok = validate_segments();
	ok &= validate_for_q();
	if(!ok) {
		cout << "NO";
	} else {
		for(auto &i: zero_idx) {
			if(i == 0) {
				// if 0 is on the 0th index, put 1
				a[i] = 1;
			} else {
				// replace 0 with element on its left
				a[i] = a[i - 1];
			}
		}
		if(mn[q] > mx[q]) a[*zero_idx.begin()] = q;
		cout << "YES\n";
		for(int i = 0 ; i < n ; i++) cout << a[i] << " ";
	}
}

#include <iostream>
#include <vector>
#include <algorithm>
#include"hashmap.h"
using namespace std;


void test() {
	int n = 20000,i;
	hashmap<int, int> m;
	for (i = 0; i < n; i++) {
		m.put(i, i);
	}
	cout << "the size of the hashmap is " << m.getSize() << endl;

	hashmap<int, int>::iterator it = m.begin();
	pair<int, int> p;
	i = 0;

	cout << "----------  test the iterator  --------------" << endl;
	while (it != m.end()) {
		i += 1;
		if (i % 200 == 0) {
			p = *it;
			cout << p.first << "," << p.second << endl;
		}
		++it;
	}

	cout << "--------------  test method contain ------------" << endl;
	for (i = 0; i < n+5; i++) {
		if (!m.contain(i)) {
			cout << "the hashmap don't contain " << i << endl;
		}
	}

	cout << "--------------  test method remove ------------" << endl;

	int x, j = min(100, n);
	cout << "try to remove " << j << " number" << endl;
	for (i = 0; i < j; i++) {
		x = rand() % n;
		m.remove(x);
		if (m.contain(x)) {
			cout << "remove number " << x << " failed" << endl;
		}
	}
	cout << "current size" << m.getSize() << endl;
}
int main() {
	int i;
	test();
	
	cin >> i;
}
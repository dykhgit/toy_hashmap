#pragma once
#include <functional>

using namespace std;

template<typename K, typename V> class hashmap {
private:
	friend struct Node;

	struct Node {
		K key;
		V val;
		int hash_val;

		Node *next;

		Node(K _key, V _val) {
			key = _key;
			val = _val;
			next = nullptr;
			hash<K> h;
			hash_val = h(key);
		}
	};

	float factor;
	int max_size;
	int capicity;             // the length of the array data
	int size;                 // the number of elments in the hashmap
	Node** data;
	hash<K> h;

	
	void reSize();
	int getHash(K _key) {
		return h(_key)&(capicity - 1);
	}

	void _reSize() {
		if (capicity*factor < size) {
			reSize();
		}
	}

public:
	hashmap(int capicity=16, float _factor=0.75);
	~hashmap();
	bool put(K _key, V _val);
	bool remove(K _key);
	bool contain(K _key);
	V get(K _key);
	int getSize() { return size; }

	

	class iterator {
	private:
		Node *p;
		const hashmap<K, V>* m;
		void next() {
			int capicity = m->capicity;
			Node** data = m->data;
			if (p->next != nullptr) {
				p = p->next;
			}
			else {
				int idx = p->hash_val & (capicity - 1);
				int i;
				for (i = idx+1; i < capicity; i++) {
					if (data[i] != nullptr) {
						p = data[i];
						return;
					}
				}
				if (i >= capicity) {
					p = nullptr;
				}
			}
		}
	public:
		iterator(const hashmap<K, V> *m, Node *pp):m(m) {
			p = pp;
		}
		iterator() {
			m = nullptr;
			p = nullptr;
		}
		pair<K, V> operator*() const {
			return pair<K, V>(p->key, p->val);
		}

		iterator &operator++() {
			next();
			return *this;
		}

		iterator operator++(int ) {
			Node *n = p;
			next();
			return iterator(this->m, n);
		}
		iterator operator=(const iterator &tag) {
			 p = tag.p;
			 m = tag.m;
			 return *this;
		}
		bool operator==(const iterator &tag) {
			return p == tag.p;
		}
		bool operator!=(const iterator &tag) {
			return p != tag.p;
		}
	};

	iterator begin() const{
		for (int i = 0; i < capicity; i++) {
			if (data[i] != nullptr) {
				
				return iterator(this,data[i]);
			}
		}
		return iterator(this,nullptr);
	}

	iterator end() {
		return iterator(this,nullptr);
	}
	
};

template<typename K, typename V>  hashmap<K, V>::hashmap(int capicity=16, float _factor=0.75) {
	max_size = 1 << 30;
	size = 0;
	if (capicity > max_size) {
		capicity = max_size;
	}
	else {
		int i = 1;
		while (capicity > i) {
			i <<= 1;
		}
		capicity = i;
	}
	factor = _factor;
	this->capicity = capicity;
	data = new Node*[capicity];
	fill(data, data + capicity, nullptr);
}

//add a (key,val) for hashmap
template<typename K, typename V> bool hashmap<K, V>::put(K _key, V _val) {
	int hash_val = getHash(_key);
	Node *p = data[hash_val];
	if (p == nullptr) {
		data[hash_val] = new Node(_key, _val);
		size += 1;
		_reSize();
		return true;
	}
	else {
		while (p != nullptr) {
			if (p->key == _key) {
				p->val = _val;
				return false;
			}
			else {
				if (p->next == nullptr) {
					p->next = new Node(_key, _val);
					size += 1;
					_reSize();
					return true;
				}
				else {
					p = p->next;
				}
			}
		}
	}
}

template<typename K, typename V> V hashmap<K, V>::get(K _key) {
	int hash = getHash(_key);
	Node *p = data[hash];
	if (p == nullptr) {
		return V();
	}
	else {
		while (p != nullptr) {
			if (p->key == _key) {
				return p->val;
			}
			p = p->next;
		}
	}
	return V();
}

// remove a key and its associated value from the hash map
// if the key is found and deleted ,return true else return false
template<typename K, typename V> bool hashmap<K, V>::remove(K _key) {
	int hash_val = getHash(_key);
	Node *p = data[hash_val], *cur;
	if (p == nullptr) return false;
	if (p->key == _key) {
		cur = p->next;
		delete p;
		data[hash_val] = cur;
		size -= 1;
		return true;
	}
	cur = p->next;
	while (cur != nullptr) {
		if (cur->key == _key) {
			p->next = cur->next;
			delete cur;
			size -= 1;
			return true;
		}
		else {
			cur = cur->next;
			p = p->next;
		}
	}
	return false;
}


template<typename K, typename V> void hashmap<K,V>::reSize() {
	int new_capicity = capicity << 1;
	int hash_val;
	if (new_capicity <= max_size) {
		Node **new_data = new Node*[new_capicity];
		fill(new_data, new_data + new_capicity, nullptr);
		Node *p = nullptr, *cur;
		for (int i = 0; i < capicity; i++) {
			p = data[i];
			while (p != nullptr) {
				hash_val = (p->hash_val) & (new_capicity - 1);
				cur = p->next;
				if (new_data[hash_val] == nullptr) {                //just insert into the head
					p->next = nullptr;
					new_data[hash_val] = p;
				}
				else {
					p->next = new_data[hash_val];
					new_data[hash_val] = p;
				}
				p = cur;
			}
		}
		capicity = new_capicity;
		delete[] data;
		data = new_data;
	}
}


template<typename K, typename V> bool hashmap<K,V>::contain(K _key) {
	int hash = getHash(_key);
	Node *p = data[hash];
	if (p == nullptr) {
		return false;
	}
	else {
		while (p != nullptr) {
			if (p->key == _key) {
				return true;
			}
			p = p->next;
		}
	}
	return false;
}

template<typename K, typename V> hashmap<K, V>::~hashmap() {
	Node *p,*cur;
	for (int i = 0; i < capicity; i++) {
		p = data[i];
		while (p!=nullptr) {
			cur = p->next;
			delete p;
			p = cur;
		}
	}
	delete[] data;
}
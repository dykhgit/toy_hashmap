#pragma once
#include "hashmap.h"

template<typename K>
class hashset
{
public:
	hashset(int n=16);
	~hashset() { delete _set; }

	bool add(K val);
	bool contain(K val);
	bool remove(K val);
	int getSize();
	

	class iterator {
	private:
		typename hashmap<K, int>::iterator _iter;

	public:
		iterator(typename hashmap<K, int>::iterator iter) { _iter = iter; }
		K operator*() {
			return (*_iter).first;
		}

		iterator &operator++() {
			++_iter;
			return *this;
		}

		iterator operator++(int) {
			typename hashmap<K, int>::iterator *old = _iter;
			++_iter;
			return iterator(old);
		}

		bool operator==(const iterator& tag) {
			return _iter == tag._iter;
		}

		bool operator!=(const iterator &tag) {
			return _iter != tag._iter;
		}
	};

	iterator begin() const {
		return iterator(_set->begin());
	}
	iterator end() const {
		return iterator(_set->end());
	}
private:
	hashmap<K, int> *_set;
};

template<typename K> hashset<K>::hashset(int capicity = 16) {
	_set = new hashmap<K, int>(capicity);
}

template<typename K> bool hashset<K>::add(K val) {
	return _set->put(val, 0);
}

template<typename K> bool hashset<K>::contain(K val) {
	return _set->contain(val);
}

template<typename K> bool hashset<K>::remove(K val) {
	if (_set->remove(val)) {
		return true;
	}
	return false;
}

template<typename K> int hashset<K>::getSize() {
	return _set->getSize();
}


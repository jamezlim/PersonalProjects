#ifndef LAB4m_H
#define LAB4m_H

#include <map>
#include <iostream>
#include <mutex>
#include <pthread.h>
#include <iterator>

template <typename K, typename V>
class ThreadSafeKVStore {
	std:: multimap < K, V> map;
	pthread_rwlock_t rwlock =  PTHREAD_RWLOCK_INITIALIZER;
	typename std:: multimap< K, V> :: iterator itr;

public:
	ThreadSafeKVStore();
	bool insert(const K key, const V value);
	bool lookup(const K key, V& value);
	bool remove(const K key);
	bool truncate( const int n);
	size_t size();
	std::pair<K,V> operator[] (const int n);
	typename std:: multimap< K, V> :: iterator begin() { return map.begin();}
	typename std:: multimap< K, V> :: iterator end() { return map.end();}
};

#include "ThreadSafeKVStore.cpp"
#endif
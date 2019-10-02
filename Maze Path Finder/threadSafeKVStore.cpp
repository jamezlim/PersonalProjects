#include "threadSafeKVStore.h"

//constructor
template <typename K, typename V>
ThreadSafeKVStore<K,V> :: ThreadSafeKVStore(){
}

template <typename K, typename V>
size_t ThreadSafeKVStore<K,V> :: size(){
	pthread_rwlock_wrlock( &rwlock);
	size_t temp = map.size();
	pthread_rwlock_unlock( &rwlock);
	return temp;
}

template <typename K, typename V>
bool ThreadSafeKVStore<K,V> :: insert(const K key, const V value){
	pthread_rwlock_wrlock( &rwlock);
	map.insert(std::pair<K,V> (key, value));
	pthread_rwlock_unlock( &rwlock);
	return true;
}

template <typename K, typename V>
bool ThreadSafeKVStore<K,V> :: lookup(const K key, V& value){
	pthread_rwlock_rdlock( &rwlock);
	itr = map.find( key);

	// if key is present 
	if ( itr != map.end()) {
		value = itr -> second;
		pthread_rwlock_unlock( &rwlock);
		return true;
	}
	else {
		std:: cout << "Fatal Error!! in lookup" << std::endl;
		pthread_rwlock_unlock( &rwlock);
		return false;
	}
}

template <typename K, typename V>
bool ThreadSafeKVStore<K,V> :: remove (const K key){
	pthread_rwlock_wrlock( &rwlock);
	if ( map.erase(key) >= 0  ){
		pthread_rwlock_unlock( &rwlock);
		return true;
	}
	else {
		std:: cout << "Fatal Error!! in remove" << std::endl;
		pthread_rwlock_unlock( &rwlock);
		return false;
	}
}

template <typename K, typename V>
bool ThreadSafeKVStore<K,V> :: truncate (const int n){
	pthread_rwlock_wrlock( &rwlock);
	// if n is 2 then we want to leave 2 elements thus erase from index 2 
	map.erase(std::next(map.begin(), n), map.end());
	pthread_rwlock_unlock( &rwlock);
	return true;
}

template <typename K, typename V>
std::pair<K,V> ThreadSafeKVStore<K,V> :: operator[] (const int n){
	pthread_rwlock_rdlock (&rwlock);
	size_t temp = map.size();
	std::pair<K,V> answer;
	// if n is past the last index return last element in map 
	if ( n >= temp ) {
		answer = *(std:: prev( map.end(),1));
	}
	else {
		// else return the n th element 
		// std:: next, prev returns a iterator so dereference 
		answer = *( std:: next( map.begin(), n)) ;
	}
	pthread_rwlock_unlock( &rwlock);
	return answer;
}

#include <iostream>

//constructor 
template <typename T>
ThreadSafeListenerQueue <T>:: ThreadSafeListenerQueue(){
	size = 0;
}

template <typename T>
void ThreadSafeListenerQueue <T> :: print(){
	for (itr = queue.begin(); itr != queue.end(); itr ++ ){
		std::cout << * itr << " --> " ;
	}
}

template <typename T>
bool ThreadSafeListenerQueue <T> :: push (const T element){
	std::unique_lock<std::mutex> lck(mtx);
	size ++;
	queue.push_back(element);
	cv.notify_all();
	return true;
}

template <typename T>
bool ThreadSafeListenerQueue <T> :: pop(T& element){

	std::unique_lock<std::mutex> lck(mtx);
	if ( size == 0 ){
		return false; 
	} 
	else if ( size != 0 ){
		element = queue.front();
		queue.pop_front();
		size--;
		return true;
	}
	// Error
	std:: cout << "Fatal Error!! in pop" << std::endl;
	return false; 
}

template <typename T>
bool ThreadSafeListenerQueue <T> :: listen(T& element){
	std::unique_lock<std::mutex> lck(mtx);
	while ( size  == 0) {
		cv.wait(lck);
	}
	if ( size != 0) {
		element = queue.front();
		queue.pop_front();
		size--;
		return true;
	}
	std:: cout << "Fatal Error!! in listen" << std::endl;
	return false;
}

template <typename T>
void ThreadSafeListenerQueue <T> :: sort(){
	queue.sort();
}

template <typename T>
T ThreadSafeListenerQueue <T> :: front(){
	return queue.front();
}

template <typename T>
T ThreadSafeListenerQueue <T> :: back(){
	return queue.back();
}



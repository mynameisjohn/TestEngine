#include "CircBuf.h"
#include <glm/glm.hpp>

template <class T>
CircBuf<T>::CircBuf(int dim){
	this->dim=dim;
	data.clear();
	data.resize(dim);
	idx=-1;
}

template <class T>
void CircBuf<T>::add(T x){
	idx=(idx+1)%dim;
	data[idx]=x;
}

template <class T>
T CircBuf<T>::last(){
	return data[idx];
}

//This is so the compiler knows to build the int version of this object
//If everything was in the header this wouldn't be necessary, but leave it
//for now.
template class CircBuf<int>;
template class CircBuf<glm::mat4>;

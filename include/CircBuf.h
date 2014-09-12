#include <vector>

template <class T>
class CircBuf{
protected:
	std::vector<T> data;
	int idx;
	int dim;
public:
	CircBuf(int dim);
/*{
		this->dim=dim;
		data.clear();
		data.resize(dim);
		idx=-1;
	}

	inline void add(T x){
		idx = (idx+1)%dim;
		data[idx] = x;
	}
	inline T last(){
		return data[idx];
	}
*/
	void add(T x);
	T last();
};

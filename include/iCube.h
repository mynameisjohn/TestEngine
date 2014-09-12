class iCube{
	public:
		iCube();
		iCube(int x, int y, int z);
		iCube(int x, int y, int z, int w, int h, int d);
	protected:
		int x, y, z;
		int w, h, d;

//inline functions
public:
	inline int left(){
		return x;
	}

	inline int right(){
		return x+w;
	}

	inline int bottom(){
		return y+h;
	}

	inline int top(){
		return y;
	}

	inline int near(){
		return z;
	}

	inline int far(){
		return z+d;
	}
};

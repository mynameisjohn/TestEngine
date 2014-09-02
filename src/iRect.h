class iRect{
	public:
		iRect();
		iRect(int w, int h);
		iRect(int x, int y, int w, int h);
		void translate(int x, int y);
		inline int bottom();
		inline int top();
		inline int left();
		inline int right();
		inline bool overlapsX(iRect);
		inline bool overlapsY(iRect);
		inline bool overlapsWith(iRect);
	private:
		int x, y, w, h;
};

inline int iRect::bottom(){
	return y+h;
}

inline int iRect::top(){
   return y;
}

inline int iRect::left(){
   return x; 
}

inline int iRect::right(){
   return x+w;
}

inline bool iRect::overlapsX(iRect other){
	return !(right() <= other.left() || left() >= other.right());
}

inline bool iRect::overlapsY(iRect other){
   return !(bottom() <= other.top() || top() >= other.bottom());
}

inline bool iRect::overlapsWith(iRect other){
	return overlapsX(other) && overlapsY(other);
}

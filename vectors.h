#ifndef __VECTORS__
template<class T> class vector3d;
template<class T>
inline vector3d<T> operator-(const vector3d<T>& a)
{
	return vector3d<T>(-a.xcoord, -a.ycoord, -a.zcoord);
}
template<class T>
inline T operator*(const vector3d<T>& a, const vector3d<T>& b)
{
	return a.xcoord * b.xcoord + a.ycoord * b.ycoord + a.zcoord * b.zcoord;
}
template<class T>
inline vector3d<T> operator&(const vector3d<T>& a, const vector3d<T>& b)
{
	return vector3d<T>(	a.ycoord * b.zcoord - a.zcoord * b.ycoord,
						a.zcoord * b.xcoord - a.xcoord * b.zcoord,
						a.xcoord * b.ycoord - a.ycoord * b.xcoord);
}

template<class T> 
inline vector3d<T> operator+(const vector3d<T>& a, const vector3d<T>& b)
{
	return vector3d<T>(a.xcoord + b.xcoord,  a.ycoord + b.ycoord, a.zcoord + b.zcoord);
}

template<class T> 
inline vector3d<T> operator-(const vector3d<T>& a, const vector3d<T>& b)
{
	return vector3d<T>(a.xcoord - b.xcoord, a.ycoord - b.ycoord, a.zcoord - b.zcoord);
}

template<class T>
inline vector3d<T> operator*(const T& a, const vector3d<T>& b)
{
	return vector3d<T>(a * b.xcoord, a * b.ycoord, a * b.zcoord);
}

template<class T>
inline vector3d<T> operator*(const vector3d<T>& a, const T& b) { return b * a; }

 template<class T>
inline vector3d<T> operator/(const vector3d<T>& a, const T& b)
{
	return vector3d<T>(a.xcoord / b, a.ycoord / b, a.zcoord / b);
}
template<class T>
class vector3d
{
	private:
		T xcoord, ycoord, zcoord;
	public:
		vector3d() { xcoord = ycoord = zcoord = T(0); }
		vector3d(const T xx, const T yy, const T zz) { xcoord = xx; ycoord = yy; zcoord = zz; }
		T x() const { return xcoord; }
		T y() const { return ycoord; }
		T z() const { return zcoord; }
		void setx( T xx ) {	xcoord = xx; }
		void sety( T yy ) {	ycoord = yy; }
		void setz( T zz ) {	zcoord = zz; }
		vector3d& operator=(const vector3d<T>& r) 
		{ xcoord = r.xcoord; ycoord = r.ycoord; zcoord = r.zcoord; return *this; }
		vector3d& operator+=(const vector3d<T>& r) 
		{ xcoord += r.xcoord; ycoord += r.ycoord; zcoord += r.zcoord; return *this; }
		vector3d& operator-=(const vector3d<T>& r) 
		{ xcoord -= r.xcoord; ycoord -= r.ycoord; zcoord -= r.zcoord; return *this; }
		vector3d& operator=(T tt) { xcoord = ycoord = zcoord = tt; return *this; }
		friend vector3d<T> operator-<>(const vector3d<T>& a);
		friend T operator*<>(const vector3d<T>& a, const vector3d<T>& b);
		friend vector3d<T> operator&<>(const vector3d<T>& a, const vector3d<T>& b);
		friend vector3d<T> operator+<>(const vector3d<T>& a, const vector3d<T>& b);
		friend vector3d<T> operator-<>(const vector3d<T>& a, const vector3d<T>& b);
		friend vector3d<T> operator*<>(const T& a, const vector3d<T>& b);
		friend vector3d<T> operator*<>(const vector3d<T>& a, const T& b);
		friend vector3d<T> operator/<>(const vector3d<T>& a, const T& b);
};

template<class T> class vector2d;
template<class T>
inline vector2d<T> operator-(const vector2d<T>& a)
{
	return vector2d<T>(-a.xcoord, -a.ycoord);
}
template<class T> 
inline T operator*(const vector2d<T>& a, const vector2d<T>& b)
{
	return a.xcoord * b.xcoord + a.ycoord * b.ycoord;
}
template<class T>
inline T operator&(const vector2d<T>& a, const vector2d<T>& b)
{
	return a.xcoord * b.ycoord - a.ycoord * b.xcoord;
}

template<class T> 
inline vector2d<T> operator+(const vector2d<T>& a, const vector2d<T>& b)
{
	return vector2d<T>(a.xcoord + b.xcoord,  a.ycoord + b.ycoord);
}

template<class T> 
inline vector2d<T> operator-(const vector2d<T>& a, const vector2d<T>& b)
{
	return vector2d<T>(a.xcoord - b.xcoord, a.ycoord - b.ycoord);
}

template<class T>
inline vector2d<T> operator*(const T& a, const vector2d<T>& b)
{
	return vector2d<T>(a * b.xcoord, a * b.ycoord);
}

template<class T>
inline vector2d<T> operator*(const vector2d<T>& a, const T& b) { return b * a; }

 template<class T>
inline vector2d<T> operator/(const vector2d<T>& a, const T& b)
{
	return vector2d<T>(a.xcoord / b, a.ycoord / b);
}
template<class T>
class vector2d
{
	private:
		T xcoord, ycoord;
	public:
		vector2d() { xcoord = ycoord = T(0); }
		vector2d(const T xx, const T yy) { xcoord = xx; ycoord = yy; }
		T x() const { return xcoord; }
		T y() const { return ycoord; }
		void setx( T xx ) {	xcoord = xx; }
		void sety( T yy ) {	ycoord = yy; }
		vector2d& operator=(const vector2d<T>& r) 
		{ xcoord = r.xcoord; ycoord = r.ycoord; return *this; }
		vector2d& operator+=(const vector2d<T>& r) 
		{ xcoord += r.xcoord; ycoord += r.ycoord; return *this; }
		vector2d& operator-=(const vector2d<T>& r) 
		{ xcoord -= r.xcoord; ycoord -= r.ycoord; return *this; }
		vector2d& operator=(T tt) { xcoord = ycoord = tt; return *this; }
		T mag(void) { return sqrt( xcoord*xcoord+ycoord*ycoord); }
		friend vector2d<T> operator-<>(const vector2d<T>& a);
		friend T operator*<>(const vector2d<T>& a, const vector2d<T>& b);
		friend T operator&<>(const vector2d<T>& a, const vector2d<T>& b);
		friend vector2d<T> operator+<>(const vector2d<T>& a, const vector2d<T>& b);
		friend vector2d<T> operator-<>(const vector2d<T>& a, const vector2d<T>& b);
		friend vector2d<T> operator*<>(const T& a, const vector2d<T>& b);
		friend vector2d<T> operator*<>(const vector2d<T>& a, const T& b);
		friend vector2d<T> operator/<>(const vector2d<T>& a, const T& b);
};
#define __VECTORS__
#endif


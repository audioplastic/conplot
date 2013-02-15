
#ifndef POINT_H
#define POINT_H

// PREPROCESSOR info for unit tests:
// #define CATCH_CONFIG_MAIN

#include <algorithm>
#include <utility>

#include "catch.hpp"


//=================================================================================
// PointData class
//=================================================================================
// Make a separate class for the point data so changes must be mase through accessors
// Gives a controlled entry point for change signals!
class PointData
{
public:
	PointData(int _x=0, int _y=0) : x(_x), y(_y) {
	}

	// RAW GETTERS/SETTERS
	int getX() const
	{
		return x;
	}
	
	int getY() const
	{
		return y;
	}
	
	void setX(int newVal)
	{
		x = newVal; //can put checks here if needed

	}
	
	void setY(int newVal)
	{
		y = newVal; //can put checks here if needed
	}
	
	void setXY(int _x, int _y) // THis function is to make fewer messages if both axes have changed
	{
		x = _x; y = _y;
	}
	

	
private:
	// Making these private means the encompassing class must use accessors
	int x, y;	

};

//---------------------------------------------------------------------------------
// PointData class end
//---------------------------------------------------------------------------------


//=================================================================================
// Point class
//=================================================================================
class Point
{
public:
	//Most basic integer ctor 
	Point(int _x=0, int _y=0) : pd(_x, _y) 
	{
		//connect(pd); //Register with underlying data object
	}
	
	// Point(const Point& p) : pd(p.pd)//explicit copying
	// {
	// 	// pd = p.pd;
	// }
	
	// Point& operator=(const Point&) =default; //explicit assignment
	
		
	
	// RAW GETTERS/SETTERS
	int getX() const
	{
		return pd.getX();
	}
	
	int getY() const
	{
		return pd.getY();
	}
	
	void setX(int newVal)
	{
		// http://oopscenities.net/2012/02/24/c11-stdfunction-and-stdbind/
		// http://oopscenities.net/2011/07/31/c0x-c-style-listener-list/
		pd.setX(newVal); //can put checks here if needed
	}
	
	void setY(int newVal)
	{
		pd.setY(newVal); //can put checks here if needed
	}
	
	void setXY(int _x, int _y)
	{
		pd.setXY(_x, _y); //can put checks here if needed
	}
	
	
	std::tuple<int,int> getLocation() const
	{
		return std::make_tuple(getX(),getY());
	}
	
	
	
	// RELATIVE GETTERS
	float getX(const Point &r) const
	{		
		float xr = static_cast<float>(getX()) / 
			static_cast<float>(r.getX()); 
		return xr;
	}
	
	float getY(const Point &r) const
	{
		float yr = static_cast<float>(getY()) / 
			static_cast<float>(r.getY()); 
		return yr;
	}
	
	std::tuple <float,float> getLocation(const Point &r) const
	{
		return std::make_tuple( getX(r), getY(r) ); //DRY - reuse single return getters
	}
	
	// RAW SETTERS	
	void move(int newX, int newY)
	{
		setX(newX);
		setY(newY);
	}
	
	void move(const std::tuple<int,int> &newLoc) 
	{
		setX(std::get<0>(newLoc));
		setY(std::get<1>(newLoc));
	}

    // RELATIVE SETTERS
	template<class T>
	void move(const Point &ref, const T fraction)
	{
		setX(ref.getX()*fraction);
		setY(ref.getY()*fraction);
	}
	
	template<class T>
	void move(const Point &ref, const T xFraction, const T yFraction)
	{
		setX(ref.getX()*xFraction);
		setY(ref.getX()*yFraction);
	}
	
	// OPERATORS (Arithmetic)
	Point operator+= (const Point other) 
	{
		setX(getX()+other.getX());
		setY(getY()+other.getY());
		return *this;
	}
	
	Point operator-= (const Point other) 
	{
		setX(getX()-other.getX());
		setY(getY()-other.getY());
		return *this;
	}
	
	Point operator+= (const int val) 
	{
		setX(getX()+val);
		setY(getY()+val);
		return *this;
	}
	
	Point operator-= (const int val) 
	{
		operator+=(-val);
		return *this;
	}
	
	//These increment X and Y independently
	//Must be tuple as += is binary
	Point operator+= (const std::tuple<int,int> inc) 
	{
		setX(getX()+std::get<0>(inc));
		setY(getY()+std::get<1>(inc));
		return *this;
	}
	
	Point operator-= (const std::tuple<int,int> dec) 
	{
		operator+=(std::make_tuple( -std::get<0>(dec) , -std::get<1>(dec) ));
		return *this;
	}
	
			
	template<class T>
	Point operator*= (const T factor) 
	{
		setX( (  static_cast<T>( getX() )*factor  ));
		setY( (  static_cast<T>( getY() )*factor  ));
		return *this;
	}
	
	template<class T>
	Point operator/= (const T factor) 
	{
		setX( static_cast<int>(  static_cast<T>( getX() ) / factor  )  );
		setY( static_cast<int>(  static_cast<T>( getY() ) / factor  )  );
		return *this;
	}
	
	//operators (Relational)
	bool operator== (const Point other) const 
	{
		// using 'this' just to be explicit
		if ( (other.getX()==this->getX()) && (other.getY()==this->getY()) ) {
			return true;
		}	else 	{
			return false;
		}					
	}
	
	bool operator!= (const Point other) const 
	{
		return !operator== (other);
	}	
	
	bool operator> (const Point other) const
	{
		// tuples only return true if all elements are satisfied
		return getLocation()>other.getLocation();
	}
	
	bool operator< (const Point other) const
	{
		// tuples only return true if all elements are satisfied
		return getLocation()<other.getLocation();
	}
	
	bool operator>= (const Point other) const
	{
		return (operator> (other)) || (operator== (other));
	}
	
	bool operator<= (const Point other) const
	{
		return (operator< (other)) || (operator== (other));
	}		
	
		
private:
	PointData pd;
};
//=================================================================================
// Point class end
//=================================================================================

//Use friend to make non-member function if you want to declare it in a class (hack)
//Or put it outside like this...

// Can use templates here as we are defining these operators in terms of +=/-=
// No need to make a function def for every type, as they will be resolved by the member functions.
template<class T>
Point operator+ (Point p1, const T &val) 
{
	return p1+=val; 
}
template<class T>
Point operator- (Point p1, const T &val) 
{
	return p1-=val; 
}

// All this commented stuff is what the above templates replace
// Point operator+ (Point p1, const Point& p2) 
// {
// 	return p1+=p2; 
// }
// Point operator- (Point p1, const Point& p2) 
// {
// 	return p1-=p2; 
// }
// Point operator+ (Point p1, const int& val) 
// {
// 	return p1+=val;
// }
// Point operator- (Point p1, const int& val) 
// {
// 	return p1-=val; 
// }
// Point operator+ (Point p1, const std::tuple<int,int> val)
// {
// 	return p1+=val;
// }
// Point operator- (Point p1, const std::tuple<int,int> val)
// {
// 	return p1-=val;
// }

//Templates used here to distinguish float/double/int etc.
template<class T>
Point operator* (Point p1, const T& factor) 
{
	return p1*=factor;
}
template<class T>
Point operator/ (Point p1, const T& factor) 
{
	return p1/=factor;
}

#endif //INCLUDE GUARD



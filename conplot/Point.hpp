
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

#ifdef DEBUG
//              _ _     _____          _       
//  /\ /\ _ __ (_) |_  /__   \___  ___| |_ ___ 
// / / \ \ '_ \| | __|   / /\/ _ \/ __| __/ __|
// \ \_/ / | | | | |_   / / |  __/\__ \ |_\__ \
//  \___/|_| |_|_|\__|  \/   \___||___/\__|___/

TEST_CASE( "Point/simple_setter", "Dumb test for simple setters" ) 
{
	Point p(1,2);
	REQUIRE(p.getX() == 1);
	REQUIRE(p.getY() == 2);	
	
	p.setX(4);
	REQUIRE(p.getX() == 4);
	
}

TEST_CASE( "Point/rel_setter", "Check relative setting capabilities" ) 
{
	Point p(1,2);
	
	p.move(10,20);
	REQUIRE(p.getX() == 10);
	REQUIRE(p.getY() == 20);
	
	p.move(p,0.5f);
	REQUIRE(p.getX() == 5);
	REQUIRE(p.getY() == 10);
	
	Point r(10,10);
	p.move(r, 1.0, 2.0);
	REQUIRE(p.getX() == 10);
	REQUIRE(p.getY() == 20);
}

TEST_CASE( "Point/rel_getter", "Check relative setting capabilities" ) 
{
	Point p(10,20);
	REQUIRE(p.getX(p) == 1.0f );
	
	Point r(5,40);
	auto tup = p.getLocation(r);
	REQUIRE(std::get<0>(tup) == 2.0f );
	REQUIRE(std::get<1>(tup) == 0.5f );
	
	// Which is almost the same as writing the neater version, but with less introspection ...
	REQUIRE(p.getLocation(r) == std::make_tuple(2.f,0.5f));
}


TEST_CASE("Point/arithmetic", "test overloaded arithmetic operators")
{
	Point p(12,13);
	p+=Point(1,2);
	REQUIRE(p.getLocation() == std::make_tuple(13,15));
	
	//Now test + from +=
	REQUIRE((p+Point(1,2)).getLocation() == std::make_tuple(14,17));
	
	p.move(14,14);
	REQUIRE(p.getLocation() == std::make_tuple(14,14));
	REQUIRE((p/2).getLocation() == std::make_tuple(7,7));
	
	p.move(10,10);
	// REQUIRE((p*1.5).getLocation() == std::make_tuple(15,15));
	REQUIRE((p*1.5).getX() == 15);
	REQUIRE((p*1.5).getY() == 15); //can be useful to split tuple to catch bugs
	
	//These change the value of the object!!
	REQUIRE( (p+=10).getLocation() == std::make_tuple(20,20) );
	REQUIRE( (p-=10).getLocation() == std::make_tuple(10,10) );	
	//These should not!!
	REQUIRE( (p+10).getLocation() == std::make_tuple(20,20) );
	REQUIRE( (p-10).getLocation() == std::make_tuple(0,0) );
	
	//Now for some tuple increments
	//These change the value of the object!!
	REQUIRE( (p+=std::make_tuple(5,10)).getLocation() == std::make_tuple(15,20) );
	REQUIRE( (p-=std::make_tuple(5,10)).getLocation() == std::make_tuple(10,10) );	
	//These should not!!
	REQUIRE( (p+std::make_tuple(5,10)).getLocation() == std::make_tuple(15,20) );
	REQUIRE( (p-std::make_tuple(5,10)).getLocation() == std::make_tuple(5,0) );
}

TEST_CASE("Point/relational", "test overloaded relational operators")
{
	REQUIRE(Point(10,10)==Point(10,10));
	REQUIRE_FALSE(Point(10,10)==Point(10,9));
	REQUIRE(Point(10,10)!=Point(10,9));
	REQUIRE_FALSE(Point(10,10)!=Point(10,10));
	
	REQUIRE(Point(10,10)>=Point(10,10));
	REQUIRE_FALSE(Point(10,10)>Point(10,10));
	
	REQUIRE(Point(10,10)<=Point(10,10));
	REQUIRE_FALSE(Point(10,10)<Point(10,10));
	
	REQUIRE(Point(10,20)>=Point(10,15));
	
	// This is an interesting test case group:
	// > to returns true if just ONE coordinate > other Point
	REQUIRE(Point(10,20)>Point(10,15));
	REQUIRE(Point(10,20)<Point(10,30));
	REQUIRE(Point(11,20)>Point(10,20));
	REQUIRE(Point(10,20)<Point(11,20));	
}



#endif //DEBUG
//======
#endif //INCLUDE GUARD



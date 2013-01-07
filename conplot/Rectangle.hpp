
#ifndef RECTANGLE_H
#define RECTANGLE_H

// PREPROCESSOR info for unit  :
// #define CATCH_CONFIG_MAIN
#include "catch.hpp"

// OTHER dependencies 
#include "Point.hpp"



//=================================================================================
// Rectangle class
//=================================================================================
class Rectangle
{
public:	
	// CTOR: Opposite corner points
	
	Rectangle() =default;
	Rectangle(const Rectangle&) =default; //explicit copying
	Rectangle& operator=(const Rectangle&) =default; //explicit assignment
	
	Rectangle (const Point &_topLeft, const Point &_btmRight) :
		topLeft(_topLeft),
		btmRight(_btmRight)
	{
	}
	
	// CTOR: Point plus a width and a height
	Rectangle(const Point &_topLeft,int _w,int _h) :
		topLeft(_topLeft),
		btmRight(_topLeft.getX()+_w-1, _topLeft.getY()+_h-1)
	{
	}
	
	// Accessors	
	std::tuple<Point, Point> getBounds() const
	{
		return std::make_tuple(topLeft, btmRight);
	}
	
	Point getLoaction() const
	{
		return topLeft;
	}
	
	Point getTL() const
	{
		return topLeft;
	}
	
	Point getBR() const
	{
		return btmRight;
	}
    
    Point getTR() const
	{
		return Point(getRight(),getTop());
	}
    
    Point getBL() const
	{
		return Point(getLeft(),getBtm());
	}
	
	Point getCentre() const
	{
		return (topLeft+btmRight) / 2.0; //get better precision with double
	}
	
	Point getRelativeCentre(const Point& p) const
	{
		return (topLeft+btmRight) / 2.0; //get better precision with double
	}
	
	// Setters
	void setLocation(const Point& newTL)
	{
		//btmRight = btmRight + (newTL-topLeft);
		//topLeft = newTL;		
		btmRight.setXY(	btmRight.getX()+newTL.getX()-topLeft.getX(),
						btmRight.getY()+newTL.getY()-topLeft.getY());
		topLeft.setXY(newTL.getX(),newTL.getY());		
	}
	
	void setLeft(const int val)
	{
		topLeft.setX(val);
	}
	
	void setRight(const int val)
	{
		btmRight.setX(val);
	}
	
	void setTop(const int val)
	{
		topLeft.setY(val);
	}
	
	void setBtm(const int val)
	{
		btmRight.setY(val);
	}
	
	void setWidth(const int val)
	{
		btmRight.setX(val+topLeft.getX()-1);
	}
	
	void setHeight(const int val)
	{
		btmRight.setY(val+topLeft.getY()-1);
	}
			
	//  Can't use tuples in function def and += here as the meaning would be ambiguous (size, location?)
	void setSize(const int width, const int height)
	{
		btmRight.setXY( topLeft.getX()+width-1, topLeft.getY()+height-1 );
	}
	
	/*void move(const int x, const int y)
	{
		topLeft.move(x,y);
		btmRight.move(x,y);
	}*/
	
	void translate(const int x, const int y)
	{
		auto shift = std::make_tuple(x, y);
		topLeft+=shift;
		btmRight+=shift;
	}
	
	void setCentre(const Point val)
	{
		Point diff = val-getCentre(); //Bit of an abuse as 'diff' is not a real point in space
		translate(diff.getX(),diff.getY());
	}
	
	//GETTERS
	int getTop() const
	{
		return topLeft.getY();
	}
	
	int getBtm() const
	{
		return btmRight.getY();
	}
	
	int getLeft() const
	{
		return topLeft.getX();
	}
	
	int getRight() const
	{
		return btmRight.getX();
	}
	
	int getWidth() const
	{
		return btmRight.getX()-topLeft.getX()+1;
	}
	
	int getHeight() const
	{
		return btmRight.getY()-topLeft.getY()+1;
	}
	
	std::tuple<int,int> getSize() const
	{
		return std::make_tuple(getWidth(), getHeight());
	}
	
	int getArea() const
	{
		return getWidth()*getHeight();
	}
	
	bool isEmpty() const
	{
		return (  (getWidth()==1)  &&  (getHeight()==1)  );
	}
	
	float getAspect() const
	{
		return static_cast<float>(getWidth()) / static_cast<float>(getHeight());
	}
	
	
	//UTILITY??
	bool contains(const Point p) const
	{
		if(	(p>=topLeft) && (p<=btmRight) ) {
			return true;
		} else {
			return false;
		}
	}
	
	bool contains(const Rectangle other) const
	{
		if(	(topLeft<=other.topLeft) && (btmRight>=other.btmRight) ) {
			return true;
		} else {
			return false;
		}
	}
	
	bool overlaps(const Rectangle other) const
	{
		// Turns out it is easier to define in terms of NOT overlapping!
		if ( 	(  getTop() > other.getBtm()  )  ||
		 		(  getBtm() < other.getTop()  )  ||
				(  getLeft() > other.getRight() )  ||
				(  getRight() < other.getLeft() ) ) {
			return false;
		} else {
			return true;
		}				
	}
	
protected:
	Point topLeft, btmRight;
};

//=================================================================================
// Rectangle class end
//=================================================================================


#ifdef DEBUG
//              _ _     _____          _       
//  /\ /\ _ __ (_) |_  /__   \___  ___| |_ ___ 
// / / \ \ '_ \| | __|   / /\/ _ \/ __| __/ __|
// \ \_/ / | | | | |_   / / |  __/\__ \ |_\__ \
//  \___/|_| |_|_|\__|  \/   \___||___/\__|___/

TEST_CASE( "Rectangle/centre", "test for centre finding" ) 
{
	Rectangle r(Point(0,0), Point(10,20));
	REQUIRE(r.getCentre() == Point(5,10));
	r.setLocation(Point(10,10));
	REQUIRE(r.getBounds() == std::make_tuple(Point(10,10),Point(20,30)));
	REQUIRE(r.getCentre() == Point(15,20));
	
	r.translate(5,5);
	REQUIRE(r.getCentre() == Point(20,25) );
	
	r.setCentre(Point(50,50));
	REQUIRE(r.getCentre() == Point(50,50) );
}

TEST_CASE( "Rectangle/Contains", "Tests for finding if rectangle contains a point" )
{
	Point p(50,50); // Make a point in space then move a rectangle about
	Rectangle r(Point(5,5), Point(15,15));
	//Testing the tuple inequalities thoroughly below
	REQUIRE_FALSE(r.contains(p));
	REQUIRE_FALSE(r.contains(p-std::make_tuple(48,40)));
	REQUIRE( r.contains(Point(8, 9)) );
	REQUIRE_FALSE( r.contains(Point(1, 50)) );
	REQUIRE_FALSE( r.contains(Point(50, 1)) );
	REQUIRE( r.contains(Point(15, 5)) );
}

TEST_CASE( "Rectangle/aspect", "aspect ratios" ) 
{
	Rectangle r;
    int w = 17;
	r.setWidth(w);
	REQUIRE( r.getCentre().getX() == 8);
	r.setHeight(9);
	r.setCentre(  Point{100,100}  );
	REQUIRE(  r.getAspect() == ((float)w/9.f)  );
	REQUIRE( r.getCentre().getX() == 100);
}

TEST_CASE( "Rectangle/overlaps", "checking rectangle overlap" ) 
{
	// There are easy cases where any corner in the rectangle obviously means overlap.
	// Can use the simple 'contains' Point method for that.
	// However, what about something like this ...?
	//
	//     +-----+
	//     |  a  |
	//   +----------+
	//   |    b     | 
	//   +----------+
	//     |     |
	//     +-----+
	//	
	Rectangle a(Point (2,0), Point(10,15));
	Rectangle b(Point (0,2), Point(12,6));
	REQUIRE(a.overlaps(b));
	
	//Now some others
	b.translate(100,0);
	REQUIRE_FALSE(a.overlaps(b));
	
	b.translate(-100,100);
	REQUIRE_FALSE(a.overlaps(b));
	
	b.setLocation(a.getCentre());
	REQUIRE(b.overlaps(a));
	
	a.setCentre(b.getCentre());
	REQUIRE(b.overlaps(a));
}

TEST_CASE( "Rectangle/contains", "checking rectangle containing" ) 
{
	Rectangle a(Point(10,10), Point (20,20));
	Rectangle b=a;
	
	REQUIRE( a.contains(b) );
	REQUIRE( b.contains(a) );
	
	b.setWidth(5);
	b.setCentre(a.getCentre());
	
	REQUIRE( a.contains(b) );
	REQUIRE_FALSE( b.contains(a) );
}

TEST_CASE( "Rectangle/default_ctor", "Check default constructor")
{
	Rectangle r;
	REQUIRE(r.isEmpty());
}

TEST_CASE( "Rectangle/ctor", "Check other constructors")
{
	Rectangle r(Point(0,0),3,4);
	REQUIRE(r.getWidth()==3);
    REQUIRE(r.getHeight()==4);

}


#endif //DEBUG
//======
#endif //INCLUDE GUARD






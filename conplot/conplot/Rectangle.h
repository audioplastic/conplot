
#ifndef RECTANGLE_H
#define RECTANGLE_H

// PREPROCESSOR info for unit  :
// #define CATCH_CONFIG_MAIN
#include "catch.h"

// OTHER dependencies 
#include "Point.h"



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


#endif //INCLUDE GUARD






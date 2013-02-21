//
//  Rectangle.cpp
//  conplot
//
//  Created by Nick on 15/02/2013.
//  Copyright (c) 2013 Nick. All rights reserved.
//
// *************
// The Unit tests now need to be placed into a CPP file.
// If they are paced in a header, they will be run multiple times
// Catch will complain of duplicate tests

#include "Rectangle.h"

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

//======

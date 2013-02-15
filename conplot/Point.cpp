//
//  Point.cpp
//  conplot
//
//  Created by Nick on 15/02/2013.
//  Copyright (c) 2013 Nick. All rights reserved.
//

// *************
// The Unit tests now need to be placed into a CPP file.
// If they are paced in a header, they will be run multiple times
// Catch will complain of duplicate tests


#include "Point.hpp"

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

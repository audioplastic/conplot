//
//  DrawToMemory.cpp
//  conplot
//
//  Created by Nick on 15/02/2013.
//  Copyright (c) 2013 Nick. All rights reserved.
//

#include "DrawToMemory.hpp"


void DataGrid::addPoint(Point p, char c)
{    
    data[p.getY()][p.getX()] = c;
}
//
//  DrawToScreen.h
//  conplot
//
//  Created by Nick on 15/02/2013.
//  Copyright (c) 2013 Nick. All rights reserved.
//

#ifndef conplot_DrawToScreen_hpp
#define conplot_DrawToScreen_hpp

#include "DrawToMemory.h"
#include "TicToc.h"

using namespace std;

//                _                                    _ _
//__   _____  ___| |_ ___  _ __  __   _____  _ __ ___ (_) |_
//\ \ / / _ \/ __| __/ _ \| '__| \ \ / / _ \| '_ ` _ \| | __|
// \ V /  __/ (__| || (_) | |     \ V / (_) | | | | | | | |_
//  \_/ \___|\___|\__\___/|_|      \_/ \___/|_| |_| |_|_|\__|
ostream& operator<<(ostream& os, DataGrid& dg)
{
    auto renderFunc = [&](){dg.render();}; // Lambda to keep things dry when render func was more complex
    if (dg.flags & (int)o_t::VERBOSE)
    {
        TicToc t;
        cout << "Rendering on stack... ";
        renderFunc();
    } else {
        renderFunc();
    }
    
    // This is the point where we act on any offset (if one exists)
    string xPad, yPad;
    xPad.assign(dg.screenDrawOffset.getX(), ' ');
    yPad.assign(dg.screenDrawOffset.getY(), '\n');
    
    
    os << endl;// New line so command prompt not in the way!
    // The following code just spews a 2D char vector to the console
    os << yPad;
    for(auto row : dg.data)
    {
        os << xPad;
        for(auto col : row){
            os << col;
        }
        os << '\n';
    }
    
    return os;
}

#endif

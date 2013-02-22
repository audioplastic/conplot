//
//  DrawToMemory.h
//  conplot
//
//  Created by Nick on 15/02/2013.
//  Copyright (c) 2013 Nick. All rights reserved.
//

#ifndef conplot_DrawToMemory_hpp
#define conplot_DrawToMemory_hpp

#include <array>
#include "DataSeries.h"
#include "Point.h"
#include "Rectangle.h"
#include "SigGen.h"

#include "CharsAndFlags.h"

using namespace std;



//     _       _
//  __| | __ _| |_ __ _
// / _` |/ _` | __/ _` |
//| (_| | (_| | || (_| |
// \__,_|\__,_|\__\__,_|
// ALl of the main logic goes here
class DataGrid
{
private:
    PlotChars pChars;
    Rectangle rect, plotArea;
	vector<vector<char>> data;
    
    Point screenDrawOffset;
    
    u_char flags;
    string title;
    
    //            _            _                        _   _               _
    // _ __  _ __(_)_   ____ _| |_ ___   _ __ ___   ___| |_| |__   ___   __| |___
    //| '_ \| '__| \ \ / / _` | __/ _ \ | '_ ` _ \ / _ \ __| '_ \ / _ \ / _` / __|
    //| |_) | |  | |\ V / (_| | ||  __/ | | | | | |  __/ |_| | | | (_) | (_| \__ \
    //| .__/|_|  |_| \_/ \__,_|\__\___| |_| |_| |_|\___|\__|_| |_|\___/ \__,_|___/
    //|_|
    void addPoint(Point p, char c);    
    void drawBox(const Rectangle& r);    
    void addString(Point p, string s);    	
	void addBorder(){drawBox(rect);}
    void addTitle();
    void addYAxis(float ymi, float yma, Rectangle axArea);
    void addXAxis(const float xmi, const float xma, const Rectangle axArea);
    void addLegend();
    
    // === This static method code maps a value within one range to a vlue in another scale
    // Both the declaration and definition of templates must be included. This is because template functions
    // cannot be compiled and linked independently, since they are generated on request for the specific
    // types they are instantiated with. That's why the implementation is not in a CPP file.
    template<typename T>
    int mapVal(T ipVal,
                         T ipMin,   T ipMax,
                         int opMin, int opMax)
    {
        //ip params
        const float ipRange = ipMax-ipMin;
        
        //prevent div 0
        if (ipRange==0)
        {
            return opMin;
        }
        
        const float ipFract = ((float)(ipVal - ipMin)) / ipRange;
        
        //op params
        const float opRange = opMax-opMin;
        return opMin + (int)round(ipFract*opRange);
    };
    
public:

    //             _     _ _        _       _             __
    // _ __  _   _| |__ | (_) ___  (_)_ __ | |_ ___ _ __ / _| __ _  ___ ___
    //| '_ \| | | | '_ \| | |/ __| | | '_ \| __/ _ \ '__| |_ / _` |/ __/ _ \
    //| |_) | |_| | |_) | | | (__  | | | | | ||  __/ |  |  _| (_| | (_|  __/
    //| .__/ \__,_|_.__/|_|_|\___| |_|_| |_|\__\___|_|  |_|  \__,_|\___\___|
    //|_|
    
    SeriesDataVec<float> series;

    // This is the important bit where the rendering happens
    friend ostream& operator<<(ostream& os, DataGrid& dg);
    
	DataGrid(Rectangle _rect = Rectangle(Point(0,0), Point(40,20))) :
    rect(_rect),
    title("unnamed"),
	data ( vector<vector<char>> (_rect.getHeight(), vector<char>(_rect.getWidth(), pChars.getChar(p_t::EMPTY)  ))  ) //ugly as
	{
        // The wierd jiggery pokery here is to move the rectangle to the top left
        // ... even if the user sets otherwise
        // it is easier to deal with positional offsets whendrawing to screen
        // ... so the desired location is stored in a separate variable.
        screenDrawOffset = rect.getTL();
        rect.setLocation(Point(0,0));
        
        plotArea = rect;
    }
    
    void setTitle(const string& _title)
    {
        title=_title;
    }
    
    void setFlags(unsigned char _flags)
    {
        flags = _flags;
    }
    
    PlotChars getChars() const
    {
        return pChars;
    }
    
    void setChars(PlotChars _ch)
    {
        pChars = _ch;
    }
    
    
    //SeriesDataVec<float>* getSeries() {return &series;}
    
    //                    _
    // _ __ ___ _ __   __| | ___ _ __
    //| '__/ _ \ '_ \ / _` |/ _ \ '__|
    //| | |  __/ | | | (_| |  __/ |
    //|_|  \___|_| |_|\__,_|\___|_|
    // This is the main rendering code
    void render();
    
};


#endif

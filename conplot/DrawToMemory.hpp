//
//  DrawToMemory.hpp
//  conplot
//
//  Created by Nick on 15/02/2013.
//  Copyright (c) 2013 Nick. All rights reserved.
//

#ifndef conplot_DrawToMemory_hpp
#define conplot_DrawToMemory_hpp

#include <array>
#include "DataSeries.hpp"
#include "Point.hpp"
#include "Rectangle.hpp"
#include "SigGen.hpp"


using namespace std;

// Put the enum classes in global scope
// Not too dangerous as it just becomes like an int or float or something
namespace  {
    enum class   p_t   {
        EMPTY = 0
        ,BORDER_L
        ,BORDER_R
        ,BORDER_T
        ,BORDER_B
        ,BORDER_TL
        ,BORDER_TR
        ,BORDER_BL
        ,BORDER_BR
        ,DATUM
        ,AXIS_V
        ,AXIS_HL
        ,AXIS_HM
        ,NUMEL
    };
    
    enum class o_t : unsigned char {
        BORDER          = 0x01 // 0x01 ==   1 == "0000 0001"
        ,XAXIS          = 0x02 // 0x02 ==   2 == "0000 0010"
        ,YAXIS          = 0x04 // 0x04 ==   4 == "0000 0100"
        ,TITLE          = 0x08 // 0x08 ==   8 == "0000 1000"
        ,LEGEND         = 0x10 // 0x10 ==  16 == "0001 0000"
        ,VERBOSE        = 0x20 // 0x20 ==  32 == "0010 0000"
        ,STEM           = 0x40 // 0x40 ==  64 == "0100 0000"
        ,RESERVED       = 0x80 // 0x80 == 128 == "1000 0000"
        //Compund flags below
        ,ALL            = 0xFF // 0xFF == 256 == "1111 1111"
        ,AXES           = 0x06 // 0x06 ==   6 == "0000 0110"
    };
}


//      _                  _                     _ _
//  ___| |__   __ _ _ __  | |__   __ _ _ __   __| | | ___ _ __
// / __| '_ \ / _` | '__| | '_ \ / _` | '_ \ / _` | |/ _ \ '__|
//| (__| | | | (_| | |    | | | | (_| | | | | (_| | |  __/ |
// \___|_| |_|\__,_|_|    |_| |_|\__,_|_| |_|\__,_|_|\___|_|
class PlotChars
{
    array<char, (int)p_t::NUMEL> charContainer; //implicit instantiation of undefined temple --> remember to include <array>!!
    
public:
    PlotChars()
    {
        charContainer[(int)p_t::EMPTY] = ' ';
        charContainer[(int)p_t::BORDER_L] = '|';
        charContainer[(int)p_t::BORDER_R] = '|';
        charContainer[(int)p_t::BORDER_T] = '~';
        charContainer[(int)p_t::BORDER_B] = '~';
        charContainer[(int)p_t::BORDER_TL] = '/';
        charContainer[(int)p_t::BORDER_TR] = '\\';
        charContainer[(int)p_t::BORDER_BL] = '\\';
        charContainer[(int)p_t::BORDER_BR] = '/';
        charContainer[(int)p_t::DATUM] = 'o';
        charContainer[(int)p_t::AXIS_HM] = '-';
        charContainer[(int)p_t::AXIS_HL] = '_';
        charContainer[(int)p_t::AXIS_V] = '|';
    }
    
#ifdef DEBUG
    ~PlotChars()
    {
        cout << "PlotChars object @ " << this << " deleted" << endl;
    }
#endif
    
    void setChar(p_t type, char c)
    {
        charContainer[(int)type] = c;
    }
    
    char getChar(p_t type)
    {
        return charContainer[(int)type];
    }
    
};

// ===============================================================================================

//     _       _
//  __| | __ _| |_ __ _
// / _` |/ _` | __/ _` |
//| (_| | (_| | || (_| |
// \__,_|\__,_|\__\__,_|
// ALl of the main logic goes here
class DataGrid
{
private:
    PlotChars chars;
    Rectangle rect, plotArea;
    SeriesDataVec<float> series;
	vector<vector<char>> data;
    
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
    
    // This static method code maps a value within one range to a vlue in another scale
    template<typename T>
    static int mapVal(T ipVal,
                      T ipMin,   T ipMax,
                      int opMin, int opMax);
    
public:
    //             _     _ _        _       _             __
    // _ __  _   _| |__ | (_) ___  (_)_ __ | |_ ___ _ __ / _| __ _  ___ ___
    //| '_ \| | | | '_ \| | |/ __| | | '_ \| __/ _ \ '__| |_ / _` |/ __/ _ \
    //| |_) | |_| | |_) | | | (__  | | | | | ||  __/ |  |  _| (_| | (_|  __/
    //| .__/ \__,_|_.__/|_|_|\___| |_|_| |_|\__\___|_|  |_|  \__,_|\___\___|
    //|_|
    
    // This is the important bit where the rendering happens
    friend ostream& operator<<(ostream& os, DataGrid& dg);
    
	DataGrid(Rectangle _rect = Rectangle(Point(0,0), Point(80,24))) :
    rect(_rect),
    plotArea(_rect),
    title("unnamed"),
	data ( vector<vector<char>> (_rect.getHeight(), vector<char>(_rect.getWidth(), chars.getChar(p_t::EMPTY)  ))  ) //ugly as shit
	{
        //plotArea = Rectangle(rect.getTL()+2, rect.getBR()-2);
    }
    
    void setTitle(const string& _title)
    {
        title=_title;
    }
    
    void setFlags(unsigned char _flags)
    {
        flags = _flags;
    }
    
    SeriesDataVec<float>* getSeries() {return &series;}
    
    //                    _
    // _ __ ___ _ __   __| | ___ _ __
    //| '__/ _ \ '_ \ / _` |/ _ \ '__|
    //| | |  __/ | | | (_| |  __/ |
    //|_|  \___|_| |_|\__,_|\___|_|
    // This is the main rendering code
    template <class T> void render();
};


#endif

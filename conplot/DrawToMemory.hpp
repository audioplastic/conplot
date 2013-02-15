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
    void addPoint(Point p, char c)
    {
        
        data[p.getY()][p.getX()] = c;
    }
    
    void drawBox(const Rectangle& r)
    {
        //Top and bottom
        fill_n(data[0].begin()+1, r.getWidth()-2, chars.getChar(p_t::BORDER_T) );
        fill_n(data[r.getBtm()].begin()+1, r.getWidth()-2, chars.getChar(p_t::BORDER_B));
        
        //Left and right hand border edges
		for (int nn=1; nn<rect.getHeight()-1; ++nn){
            addPoint(Point(r.getLeft(), nn), chars.getChar(p_t::BORDER_L) );
            addPoint(Point(r.getRight(), nn), chars.getChar(p_t::BORDER_R) );
        }
        
        //Corners
        addPoint(r.getTL(), chars.getChar(p_t::BORDER_TL) );
        addPoint(r.getTR(), chars.getChar(p_t::BORDER_TR) );
        addPoint(r.getBL(), chars.getChar(p_t::BORDER_BL) );
        addPoint(r.getBR(), chars.getChar(p_t::BORDER_BR) );
    }
    
    
    void addString(Point p, string s)
    {
        if ( s.length()>(rect.getBR()-p).getX() ){
            cout << "WARNING: The string '" << s << "' is too long to fit in the plot area.\n";
            return;
        }
        
        int x = p.getX();
        int y = p.getY();
        for( char c : s)
            data[y][x++]=c;
    }
    
	
	void addBorder()
	{
        drawBox(rect);
	}
    
    void addLegend()
	{
        //        //Make a rectangle size that fits series data names and then plce it
        //        const size_t maxw = plotArea.getWidth()-4;
        //        const size_t maxh = plotArea.getHeight()-4;
        //
        //        const size_t hNeeded = series.getNumSeries() + 2;
        //        if (hNeeded>maxh)
        //        {
        //            cout << "Warning: Not enough veritcal space for legend";
        //        }
        //
        //        size_t wNeeded = 0;
        //        for (auto tmpSeries : series)
        //        {
        //
        //        }
        
	}
    
    
    void addTitle()
    {
        // Truncate if the title is huge
        size_t maxw = rect.getWidth()-9;
        if (title.length()>maxw) {
            title.resize(maxw);
            title += string("...");
        }
        title.insert(0, string(" "));
        title.append(string(" "));
        
        
        addString(Point(2,0), title);
    }
    
    void addYAxis(float ymi, float yma, Rectangle axArea)
    {
        // generate a yAxis vector
        const size_t yPts = axArea.getHeight();
        vector<float> yAnnot(yPts);
        
        // These two lines make equivalet of MAtlab's linspace(start,end,points)
        std::generate_n(yAnnot.begin(), yPts-1, gen_lin(ymi,(yma-ymi)/(yPts-1)) );
        yAnnot.back()=yma;
        
        
        
        // This is so ugly!! Refactor!
        size_t counter=yPts; //Decreasing counter
        for(int nn=axArea.getTop(); nn<axArea.getBtm()+1; ++nn){
            addPoint(Point(axArea.getRight(),nn),chars.getChar(p_t::AXIS_V));
            ostringstream v; // Make this every time as clearing it is a PITA
            v.width(9);
            v.precision(2);
            if (    min(fabs(yma),fabs(ymi)) < 0.05    ||    max(fabs(yma),fabs(ymi)) > 1e5   ) {
                v << scientific << yAnnot[--counter];
            } else {
                v << fixed << yAnnot[--counter];
            }
            addString(Point(axArea.getLeft(),nn), v.str()  );
        }
        
    }
    
    void addXAxis(const float xmi, const float xma, const Rectangle axArea)
    {
        addString(Point(axArea.getLeft(), axArea.getBtm()+1), "^" );
        addString(Point(axArea.getRight(), axArea.getBtm()+1), "^" );
        
        const size_t min_prec = 4;
        
        if (axArea.getWidth() > 2*min_prec+4) // Only put numerice values if there is space
        {
            const size_t prec = axArea.getWidth()-4;
            char buf[prec+1]; // The +1 contains the trunctaion warning
            size_t l; //The "desired" string length returned from snprintf;
            
            l = std::snprintf(buf, prec, "%g", xmi);
            if (l > prec) buf[prec-1] = 't'; // Puts a t to flag truncation
            addString(Point(axArea.getLeft()+1, axArea.getBtm()+1), buf);
            
            l = std::snprintf(buf, prec, "%g", xma);
            if (l > prec) buf[prec-1] = 't';// Puts a t to flag truncation
            addString(Point(axArea.getRight()-(int)std::min(l,prec), axArea.getBtm()+1), buf);
        }
    }
    
    
    // _ __ __ _ _ __   __ _  ___   _ __ ___   __ _ _ __
    //| '__/ _` | '_ \ / _` |/ _ \ | '_ ` _ \ / _` | '_ \
    //| | | (_| | | | | (_| |  __/ | | | | | | (_| | |_) |
    //|_|  \__,_|_| |_|\__, |\___| |_| |_| |_|\__,_| .__/
    //                 |___/                       |_|
    // This code maps a value within one range to a vlue in another scale
    template<typename T>
    static int mapVal(T ipVal,
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
    template <class T>
    void render()
    {
        //vector<T> xData(_xData); //Assign xData to non-const so we can pass by const reference.
        
        // Add the border if flagged
        if (flags & (int)o_t::BORDER) {
            addBorder();
            const int shrink = 4;
            plotArea.setSize(plotArea.getWidth()-shrink, plotArea.getHeight()-shrink);
            plotArea.translate(shrink/2, shrink/2);
        }
        
        // Add the title if flagged
        if (flags & (int)o_t::TITLE) {
            addTitle();
        }
        
        // Calculate ranges
        //const T xMin = *std::min_element(xData.begin(), xData.end());
        //const T xMax = *std::max_element(xData.begin(), xData.end());
        //const T yMin = *std::min_element(yData.begin(), yData.end());
        //const T yMax = *std::max_element(yData.begin(), yData.end());
        
        // Abort if series vec is empty
        if (series.getNumSeries()==0){
            cout << "No data series available, aborting. \n";
            return;
        }
        
        // get ranges
        const T xMin = series.getXmin();
        const T xMax = series.getXmax();
        const T yMin = series.getYmin();
        const T yMax = series.getYmax();
        
        
        
        // Add the y-axis if flagged
        if (flags & (int)o_t::YAXIS) {
            const int yaxWidth = 10;
            Rectangle rAx = Rectangle(plotArea.getTL(), yaxWidth, plotArea.getHeight());
            addYAxis(yMin, yMax, rAx);
            
            // Shift the plot area
            plotArea.setWidth(plotArea.getWidth()-yaxWidth);
            plotArea.translate(yaxWidth, 0);
        }
        
        // Add simple x-axis indicators if requested
        if (flags & (int)o_t::XAXIS) {
            addXAxis(xMin, xMax, plotArea);
        }
        
        
        for (size_t ii=0; ii<series.getNumSeries(); ++ii){
            // Warn if input is messed up
            vector<float> xData(series.getXdata(ii));
            vector<float> yData(series.getYdata(ii));
            
            if (xData.size()!=yData.size()) {
                cout << "WARNING: x and y data size mismatch, not plotting.\n";
                return;
            }
            
            
            for(size_t nn=0; nn<xData.size(); ++nn){
                int intX = mapVal(   xData[nn], xMin, xMax, plotArea.getLeft(), plotArea.getRight()   );
                // Notice the subtle reversal of yMax and yMin here so data flipped the correct way
                int intY = mapVal(   yData[nn], yMax, yMin, plotArea.getTop(), plotArea.getBtm()   );
                addPoint(Point(intX, intY), series.getMarker(ii) );
            }
        }
        
        // Add the legend if flagged
        if (flags & (int)o_t::LEGEND) {
            addLegend();
        }
        
        
    } // End of rendering function
	
};


#endif

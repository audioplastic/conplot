
// This needs to go right at the top
#define CATCH_CONFIG_RUNNER

#include <vector>
#include <iostream>
#include <array>
#include <random>
#include <algorithm>
#include <chrono>

#include "Point.hpp"
#include "Rectangle.hpp"
#include "SigGen.h"

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
        ,AXIS_H
        ,NUMEL
    };
    
    enum class o_t : unsigned char {
        BORDER          = 0x01 // 0x01 ==   1 == "0000 0001"
        ,AXES           = 0x02 // 0x02 ==   2 == "0000 0010"
        ,STEM           = 0x04 // 0x04 ==   4 == "0000 0100"
        ,TITLE          = 0x08 // 0x08 ==   8 == "0000 1000"
        ,LEGEND         = 0x10 // 0x10 ==  16 == "0001 0000"
        ,RESERVED2      = 0x20 // 0x20 ==  32 == "0010 0000"
        ,RESERVED3      = 0x40 // 0x40 ==  64 == "0100 0000"
        ,RESERVED4      = 0x80 // 0x80 == 128 == "1000 0000"
        ,ALL            = 0xFF // 0xFF == 256 == "1111 1111"
    };
}

// TicToc RAII profiler
class TicToc
{
private:
    typedef std::chrono::high_resolution_clock clock;
    typedef std::chrono::microseconds res;
    clock::time_point t1, t2;
    
public:
    TicToc() {
        t1 = clock::now();
    }
    
    ~TicToc()
    {
        t2 = clock::now();
        std::cout << "Elapsed time is "
        << std::chrono::duration_cast<res>(t2-t1).count()/1e6
        << " seconds.\n"; 
    }
};

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
        charContainer[(int)p_t::AXIS_H] = '-';
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
 
// __           _                 _       _
/// _\ ___ _ __(_) ___  ___    __| | __ _| |_ __ _
//\ \ / _ \ '__| |/ _ \/ __|  / _` |/ _` | __/ _` |
//_\ \  __/ |  | |  __/\__ \ | (_| | (_| | || (_| |
//\__/\___|_|  |_|\___||___/  \__,_|\__,_|\__\__,_|
template <class T>
class SeriesData
{
private:
    vector<T> yData, xData;
    char marker;
    T xMin, xMax, yMin, yMax;
    
public:    
    SeriesData(const vector<T>& _yData,  const vector<T>& _xData = vector<T>()) :
    marker('*'),
    xData(_xData), yData(_yData)
    {
        // Warn if input is messed up
        if (xData.size()>0 && xData.size()!=yData.size()) {
            cout << "WARNING: when creating data series: x and y data size mismatch.\n";
            return;
        }
        
        // Make the x vector if not supplied
        if (xData.empty()) {
            int x = 0;
            for (auto dummy: yData){
                xData.push_back( (T)++x );
            }
        }
        
        // Calculate ranges
        xMin = *std::min_element(xData.begin(), xData.end());
        xMax = *std::max_element(xData.begin(), xData.end());
        yMin = *std::min_element(yData.begin(), yData.end());
        yMax = *std::max_element(yData.begin(), yData.end());
    }
    
    // Get Accessors
    T getXmin() const {return xMin;}
    T getXmax() const {return xMax;}
    T getYmin() const {return yMin;}
    T getYmax() const {return yMax;}
    
    const vector<T>& getXdata() const {return xData;}
    const vector<T>& getYdata() const {return yData;}
    
    void setMarker (const char& _marker)
    {
        marker = _marker;
    }
};
    
// __           _                 _       _
/// _\ ___ _ __(_) ___  ___    __| | __ _| |_ __ _  __   _____  ___
//\ \ / _ \ '__| |/ _ \/ __|  / _` |/ _` | __/ _` | \ \ / / _ \/ __|
//_\ \  __/ |  | |  __/\__ \ | (_| | (_| | || (_| |  \ V /  __/ (__
//\__/\___|_|  |_|\___||___/  \__,_|\__,_|\__\__,_|   \_/ \___|\___|
// Not ideal atm as whole vector needs to be of the same type.
template <class T>
class SeriesDataVec
{
private:
    vector<SeriesData<T>> seriesVec;
    vector<string> labelsVec;
    vector<char> markersVec;
    
public:
    void addSeries(const SeriesData<T>& _newSeries, char marker = '?', string label = "")
    {
        seriesVec.push_back( _newSeries);
        
        if (label.empty()) {
            label = string("series ") + to_string(seriesVec.size());
        }
        if (  !to_string(marker).compare( to_string('?') )   ) {
            string tmp = to_string(seriesVec.size());
            marker = *tmp.c_str();
        }
        
        labelsVec.push_back( label );
        markersVec.push_back( marker );
    }
    
    void clear() {seriesVec.clear();};
    
    // Not very DRY
    float getXmin()
    {
        vector<T> tmpVal;
        for (auto s : seriesVec){
            tmpVal.push_back(  s.getXmin()  );
        }
        if(!tmpVal.empty()) {
            return *std::min_element(tmpVal.begin(), tmpVal.end());
        } else {
            return 0;
        }
    }
    
    float getXmax()
    {
        vector<T> tmpVal;
        for (auto s : seriesVec){
            tmpVal.push_back(  s.getXmax()  );
        }
        return *std::max_element(tmpVal.begin(), tmpVal.end());
    }
    
    float getYmin()
    {
        vector<T> tmpVal;
        for (auto s : seriesVec){
            tmpVal.push_back(  s.getYmin()  );
        }
        return *std::min_element(tmpVal.begin(), tmpVal.end());
    }
    
    float getYmax()
    {
        vector<T> tmpVal;
        for (auto s : seriesVec){
            tmpVal.push_back(  s.getYmax()  );
        }
        return *std::max_element(tmpVal.begin(), tmpVal.end());
    }
    
    string getLabel(const int idx)
    {
        return labelsVec[index];
    }
    
    const vector<T>& getXdata(const size_t idx) const
    {
        return seriesVec[idx].getXdata();
    }
    
    const vector<T>& getYdata(const size_t idx) const
    {
        return seriesVec[idx].getYdata();
    }
    
    const char& getMarker(const size_t idx) const
    {
        return markersVec[idx];
    }
    
    size_t getNumSeries() const {return seriesVec.size();}
    
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
    shared_ptr<PlotChars> chars;
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
        fill_n(data[0].begin()+1, r.getWidth()-2, chars->getChar(p_t::BORDER_T) );
        fill_n(data[r.getBtm()].begin()+1, r.getWidth()-2, chars->getChar(p_t::BORDER_B));
        
        //Left and right hand border edges
		for (int nn=1; nn<rect.getHeight()-1; ++nn){
            addPoint(Point(r.getLeft(), nn), chars->getChar(p_t::BORDER_L) );
            addPoint(Point(r.getRight(), nn), chars->getChar(p_t::BORDER_R) );
        }
        
        //Corners
        addPoint(r.getTL(), chars->getChar(p_t::BORDER_TL) );
        addPoint(r.getTR(), chars->getChar(p_t::BORDER_TR) );
        addPoint(r.getBL(), chars->getChar(p_t::BORDER_BL) );
        addPoint(r.getBR(), chars->getChar(p_t::BORDER_BR) );
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
        //Make a rectangle size that fits series data names and then plce it
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
            addPoint(Point(axArea.getRight(),nn),chars->getChar(p_t::AXIS_V));
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
    
	DataGrid(shared_ptr<PlotChars> _chars, Rectangle _rect = Rectangle(Point(0,0), Point(80,24))) :
    chars(_chars),
    rect(_rect),
    plotArea(_rect),
    title("unnamed"),
	data ( vector<vector<char>> (_rect.getHeight(), vector<char>(_rect.getWidth(), chars->getChar(p_t::EMPTY)  ))  ) //ugly as shit
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
        if (flags & (int)o_t::AXES) {
            const int yaxWidth = 10;
            Rectangle rAx = Rectangle(plotArea.getTL(), yaxWidth, plotArea.getHeight());
            addYAxis(yMin, yMax, rAx);
            
            // Shift the plot area 
            plotArea.setWidth(plotArea.getWidth()-yaxWidth);
            plotArea.translate(yaxWidth, 0);
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




//                _                                    _ _
//__   _____  ___| |_ ___  _ __  __   _____  _ __ ___ (_) |_
//\ \ / / _ \/ __| __/ _ \| '__| \ \ / / _ \| '_ ` _ \| | __|
// \ V /  __/ (__| || (_) | |     \ V / (_) | | | | | | | |_
//  \_/ \___|\___|\__\___/|_|      \_/ \___/|_| |_| |_|_|\__|
ostream& operator<<(ostream& os, DataGrid& dg)
{
    
    {
        cout << "Rendering in memory... ";
        TicToc t;
        dg.render<float>();
    }

    os << endl;// New line so command prompt not in the way!
    // The following code just spews a 2D char vector to the console
    for(auto row : dg.data)
    {
        for(auto col : row){
            os << col;
        }
        os << '\n';
    }
    
    return os;
}




//                 _
// _ __ ___   __ _(_)_ __
//| '_ ` _ \ / _` | | '_ \
//| | | | | | (_| | | | | |
//|_| |_| |_|\__,_|_|_| |_|
int main(int argc, char *argv[])
{
    const size_t elements = 300;
    std::vector<float> y(elements);
    std::vector<float> x(elements);
    
    
//    std::uniform_real_distribution<float> distribution(0.0f, 2.0f); //Values between 0 and 2
//    std::mt19937 engine; // Mersenne twister MT19937
//    auto generator = std::bind(distribution, engine);
//    std::generate_n(y.begin(), elements, generator);
    
//    std::generate_n(y.begin(), elements, gen_sine(2 * 2.f*M_PI/elements) );
//    std::generate_n(y.begin(), elements, gen_square(elements / 3,0.1f) );
//    std::generate_n(y.begin(), elements, gen_trisaw(elements / 3, 1.0f) );
    
    auto a = gen_sine(2.f*M_PI/elements); auto b = gen_sine(3 * 2.f*M_PI/elements);
    std::generate_n(y.begin(), elements, [&](){ return (a() + b()); } );
    
    
    
    std::generate_n(x.begin(), elements, gen_lin(10.f,5.f) );
//    auto lin = gen_lin(10.f,5.f);
//    std::generate_n(x.begin(), elements, [&](){return logf(lin());} );
    
    
    
    shared_ptr<PlotChars> ch (new PlotChars());
    DataGrid g = DataGrid(ch, Rectangle(Point(0,0), 80, 30)); // Half console
    ch->setChar(p_t::BORDER_T, '~');
    ch->setChar(p_t::BORDER_B, '~');
    
    g.getSeries()->addSeries(   SeriesData<float>(y,x) , '*' );
    
    std::vector<float> y2(elements);
    std::generate_n(y2.begin(), elements, [&](){return .5*a();} );
    g.getSeries()->addSeries(   SeriesData<float>(y2,x)  );
    
    std::vector<float> y3(elements);
    std::generate_n(y3.begin(), elements, [&](){return -a();} );
    g.getSeries()->addSeries(   SeriesData<float>(y3,x), '>'  );
    
    
    g.setTitle("Spongeballs");
    g.setFlags ((u_char)o_t::ALL); //((int)o_t::border | (int)o_t::title);
    
	cout << g;
    
    
#ifdef DEBUG
	// Create a default config object
    Catch::Config config;
    
    // Configure CATCH to send all its output to a stringstream
    std::ostringstream oss;
    config.setStreamBuf( oss.rdbuf() );
    
    // Forward on to CATCH's main, but using our custom config.
    // CATCH will still parse the command line and set the config
    // object up further
    int result = Catch::Main( argc, argv, config );
    
    std::cout << oss.str() << std::endl;
    
    return result;
#endif
    
    return 0;
}



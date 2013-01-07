
// This needs to go right at the top
#define CATCH_CONFIG_RUNNER


#include <vector>
#include <iostream>
#include <algorithm>
#include "Point.hpp"
#include "Rectangle.hpp"

using namespace std;

// Put the enum in global scope
// Not too dangerous as it just becomes like an int or float or something
enum p_t   {
    EMPTY=0
    ,BORDER_L
    ,BORDER_R
    ,BORDER_T
    ,BORDER_B
    ,BORDER_TL
    ,BORDER_TR
    ,BORDER_BL
    ,BORDER_BR
    ,DATUM
    ,STEM
    ,numel    };



//     _       _
//  __| | __ _| |_ __ _
// / _` |/ _` | __/ _` |
//| (_| | (_| | || (_| |
// \__,_|\__,_|\__\__,_|
// ALl of the main logic goes here
class DataGrid
{
public:
    friend ostream& operator<<(ostream& os, const DataGrid& dg);
    
	DataGrid(Rectangle _rect = Rectangle(Point(0,0), Point(5,5))) :
    rect(_rect),
	data ( vector<vector<p_t>> (_rect.getHeight(), vector<p_t>(_rect.getWidth(), p_t::EMPTY))  ) //ugly as shit
	{
		//gfxString = string(u8" ||--+") + string(u8"+") + string(u8"++ijklmnop");
	}
    
    void addPoint(Point p, p_t type)
    {
        
        data[p.getY()][p.getX()] = type;
    }
	
	void addBorder()
	{
        //Top and bottom
        fill_n(data[0].begin()+1, rect.getWidth()-2, p_t::BORDER_T);
        fill_n(data[rect.getBtm()].begin()+1, rect.getWidth()-2, p_t::BORDER_B);
        
        //Left and right hand border edges
		for (int nn=1; nn<rect.getHeight()-1; ++nn){
            addPoint(Point(rect.getLeft(), nn), p_t::BORDER_L);
            addPoint(Point(rect.getRight(), nn), p_t::BORDER_R);
        }
        
        //Corners
        addPoint(rect.getTL(), p_t::BORDER_TL);
        addPoint(rect.getTR(), p_t::BORDER_TR);
        addPoint(rect.getBL(), p_t::BORDER_BL);
        addPoint(rect.getBR(), p_t::BORDER_BR);
	}
		
//    void addTitle(const string& title)
//    {
//        if (title.length()>rect.getWidth()-5)
//            
//    }
	
private:
	Rectangle rect;
	vector<vector<p_t>> data;
    
    vector<string> gfxString  {
        " "
        , u8"\u2502"
        , u8"\u2502"
        , u8"\u2500"
        , u8"\u2500"
        , u8"\u250C"
        , u8"\u2510"
        , u8"\u2514"
        , u8"\u2518"
    };
	//string gfxString;
};



//                    _
// _ __ ___ _ __   __| | ___ _ __
//| '__/ _ \ '_ \ / _` |/ _ \ '__|
//| | |  __/ | | | (_| |  __/ |
//|_|  \___|_| |_|\__,_|\___|_|
// This is the main rendering code
ostream& operator<<(ostream& os, const DataGrid& dg)
{
    for(auto row : dg.data)
    {
        string tmp; //Make empty string
        for(auto col : row){
            tmp +=dg.gfxString[col];
            //Throw in some if statements here for certain lines for title and numbers
        }
        os << tmp << '\n';
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
	vector<int> y{1,2,3,1,2,3};
	vector<int> x{1,2,3,4,5,6};
	
    
    DataGrid p = DataGrid(Rectangle(Point(0,0), 20, 10));

    p.addBorder();
    
	cout << p;
	cout << u8"\u256C\u256C\u256C\n";
    cout << "œ∑´®†¥¨^øπåß∂ƒ©˙˚¬Ω≈ç√∫~µ≤≥\n";
    
    
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
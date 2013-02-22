
// This needs to go right at the top
#define CATCH_CONFIG_RUNNER

#include "conplot.h"
#include <random>

using namespace std;


//                 _
// _ __ ___   __ _(_)_ __
//| '_ ` _ \ / _` | | '_ \
//| | | | | | (_| | | | | |
//|_| |_| |_|\__,_|_|_| |_|
int main(int argc, char *argv[])
{
    //--------------------------------------------------------------------
    cout << "\n - DEMO 1: Plot some noise\n";
    {
        const size_t elements = 200;
        std::vector<float> y(elements);
        
        std::uniform_real_distribution<float> distribution(0.0f, 2.0f); //Values between 0 and 2
        std::mt19937 engine; // Mersenne twister MT19937
        auto r = std::bind(distribution, engine);
        std::generate_n(y.begin(), elements, r);
        
        DataGrid g;
        g.series.addSeries(   SeriesData<float>(y) , '*' );
        cout << g;
    }
    
    //--------------------------------------------------------------------
    cout << "\n - DEMO 2: Plot a sine wave\n";
    {
        const size_t elements = 200;
        std::vector<float> y(elements);
                
        auto a = gen_sine(2.f*M_PI/elements);
        std::generate_n(y.begin(), elements, a);
    
        DataGrid g;
        g.series.addSeries(   SeriesData<float>(y) , '*' );
        cout << g;
    }
    
    //--------------------------------------------------------------------
    cout << "\n - DEMO 3: Logarithmic axis\n";
    {
        const size_t elements = 200;
        std::vector<float> y(elements);
        std::vector<float> x(elements);
        
        auto a = gen_sine(2.f*M_PI/elements);
        std::generate_n(y.begin(), elements, a);
        
        auto lin = gen_lin(10.f,5.f);
        std::generate_n(x.begin(), elements, [&](){return logf(lin());} );
        
        DataGrid g;
        g.series.addSeries(   SeriesData<float>(y,x) , '*' );
        cout << g;
    }
    
    //--------------------------------------------------------------------
    cout << "\n - DEMO 4: Plot multiple series on same area\n";
    {
        const size_t elements = 300;
        std::vector<float> y1(elements);
        std::vector<float> y2(elements);
        std::vector<float> y3(elements);
        std::vector<float> x(elements);
        
        std::generate_n(x.begin(), elements, gen_lin(100.f, 100.f) );

        auto a = gen_sine(2.f*M_PI/elements);
        auto b = gen_sine(3 * 2.f*M_PI/elements);
        std::generate_n(y1.begin(), elements, [&](){ return (a() + b()); } );
        std::generate_n(y2.begin(), elements, [&](){return .5*a();} );
        std::generate_n(y3.begin(), elements, [&](){return -a();} );
        
        DataGrid g = DataGrid(Rectangle(Point(0,0), 80, 30)); //Customize size with rectangle object
        g.series.addSeries(   SeriesData<float>(y1,x) , '*');
        g.series.addSeries(   SeriesData<float>(y2,x)  ); // if no marker specified, it will use series number
        g.series.addSeries(   SeriesData<float>(y3,x), '>'  );

        g.setFlags ((u_char)o_t::ALL);
        cout << g;
    }
    
    //--------------------------------------------------------------------
    cout << "\n - DEMO 5: Subtle tweaks\n";
    {
        const size_t elements = 100;
        std::vector<float> y(elements);
        
        auto a = gen_sine(2.f*M_PI/elements);
        std::generate_n(y.begin(), elements, a);
        
        DataGrid g = DataGrid(Rectangle(Point(20,3), 50, 15));
        g.series.addSeries(   SeriesData<float>(y) , '*' );
        
        g.setFlags ((u_char)o_t::BORDER | (u_char)o_t::TITLE);
        g.setTitle("SOME AMAZING PLOT TITLE");
        PlotChars tmpChars = g.getChars();
        tmpChars.setChar(p_t::BORDER_T, '~');
        tmpChars.setChar(p_t::BORDER_B, '~');
        tmpChars.setChar(p_t::BORDER_L, '[');
        tmpChars.setChar(p_t::BORDER_R, ']');
        g.setChars(tmpChars);
        
        cout << g;
    }
    
//#define CONPLOT_UNIT_TEST
    
#ifdef CONPLOT_UNIT_TEST
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



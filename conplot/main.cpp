
// This needs to go right at the top
#define CATCH_CONFIG_RUNNER

#include <vector>
#include <iostream>
#include <random>
#include <algorithm>


#include "Point.hpp"
#include "Rectangle.hpp"
#include "SigGen.hpp"

#include "DrawToScreen.hpp"

using namespace std;



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
    
    
    
    std::generate_n(x.begin(), elements, gen_lin(100.f, 100.f) );
//    auto lin = gen_lin(10.f,5.f);
//    std::generate_n(x.begin(), elements, [&](){return logf(lin());} );
    
    
    
    DataGrid g = DataGrid(Rectangle(Point(0,0), 90, 31)); // Half console
//    ch->setChar(p_t::BORDER_T, '~');
//    ch->setChar(p_t::BORDER_B, '~');
    
    g.getSeries()->addSeries(   SeriesData<float>(y,x) , '*', "Series A" );
    
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



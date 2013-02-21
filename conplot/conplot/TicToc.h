//
//  TicToc.h
//  conplot
//
//  Created by Nick on 15/02/2013.
//  Copyright (c) 2013 Nick. All rights reserved.
//

#ifndef conplot_TicToc_h
#define conplot_TicToc_h

#include <chrono>
#include <iostream>

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
        << 1000*(std::chrono::duration_cast<res>(t2-t1).count()/1e6)
        << " milliseconds.\n";
    }
};

#endif

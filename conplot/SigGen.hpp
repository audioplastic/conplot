//
//  SigGen.h
//  conplot
//
//  Created by Nick on 23/12/2012.
//  Copyright (c) 2012 Nick. All rights reserved.
//

#ifndef conplot_SigGen_h
#define conplot_SigGen_h

#include <array>


//     _                   _    __                  _
// ___(_) __ _ _ __   __ _| |  / _|_   _ _ __   ___| |_ ___  _ __ ___
/// __| |/ _` | '_ \ / _` | | | |_| | | | '_ \ / __| __/ _ \| '__/ __|
//\__ \ | (_| | | | | (_| | | |  _| |_| | | | | (__| || (_) | |  \__ \
//|___/_|\__, |_| |_|\__,_|_| |_|  \__,_|_| |_|\___|\__\___/|_|  |___/
//       |___/
//Random range generating functor
//call with std::generate_n(x.begin(), num_items, gen_rand(min,max));
//or with std::generate_n(std::back_inserter(x), num_items, gen_rand(min,max));
// OBSOLETE
//template<typename T>
//class gen_rand {
//    float range, factor, norm;
//public:
//    gen_rand(float mi=0.f, float ma=1.f) : range(ma-mi), factor(range/RAND_MAX), norm(mi) {}
//    T operator()() {
//        return  rand() * factor + norm;
//    }
//};

//Sine generating functor
#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif
class gen_sine {
    float dRad, mi, ma,  w;
public:
    gen_sine(float _dRad = 2.f*M_PI/100, float _mi=-1.f, float _ma=1.f, float _w=0*M_PI) :
    dRad(_dRad),
    mi(_mi),
    ma(_ma),
    w(_w){}
    
    float operator()() {
        return  ((ma-mi)/2.f) * (1+sinf(w+=dRad)) + mi;
    }
};

//Linear spaced vector generator
class gen_lin {
    float mi, inc;
public:
    gen_lin(float _mi=1.f, float _inc=1.f) : mi(_mi-_inc), inc(_inc){};
    float operator()() {
        return  mi+=inc;
    }
};

// Square wave generator
class gen_square {
    int  counter;// U and d are up and down cycles
    bool isDown = false;
    std::array<int,2> stateSamples;
    std::array<float,2> stateValues;
    
public:
    gen_square(float period = 20, float duty = 0.5f, int _counter = 0, float _mi=-1.f, float _ma=1.f) :
    stateSamples {{  (int)round(period*duty),  (int)round(period*(1.f-duty))  }},
    counter(_counter),
    stateValues {{  _ma, _mi   }}
    {}
    
    float operator()() {
        if (counter>stateSamples[(int)isDown]){
            isDown = !isDown;
            counter=0;
        }
        ++counter;
        return  stateValues[(int)isDown];
    }
};

// TriSaw generator
class gen_trisaw {
    int  counter;
    bool isDown = false;
    float ma, mi;
    std::array<int,2> stateSamples;
    std::array<float,2> stateSlopes;
    float lastOp;
    
    
public:
    gen_trisaw(float period = 20.f, float duty = 0.5f, float _mi=-1.f, float _ma=1.f) :
    ma(_ma), mi(_mi),
    lastOp(_mi)
    {
        float range(_ma-_mi);
        stateSamples[0] = (int)round(period*duty);
        stateSamples[1] = (int)round(period*(1.f-duty));
        
        stateSlopes[0] = range/fmax(stateSamples[0], 1); //max to stop div0
        stateSlopes[1] = -range/fmax(stateSamples[1], 1);
    }
    
    float operator()() {
        if (counter==stateSamples[(int)isDown]){
            isDown = !isDown;
            counter=0;
            return  lastOp+=stateSlopes[isDown]; //needed incase samples=0
        }
        ++counter;
        return  lastOp+=stateSlopes[isDown];
    }
};



#endif

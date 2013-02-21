//
//  CharsAndFlags.hpp
//  conplot
//
//  Created by Nick on 21/02/2013.
//  Copyright (c) 2013 Nick. All rights reserved.
//

#ifndef conplot_CharsAndFlags_hpp
#define conplot_CharsAndFlags_hpp

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
        charContainer[(int)p_t::BORDER_T] = '-';
        charContainer[(int)p_t::BORDER_B] = '-';
        charContainer[(int)p_t::BORDER_TL] = '/';
        charContainer[(int)p_t::BORDER_TR] = '\\';
        charContainer[(int)p_t::BORDER_BL] = '\\';
        charContainer[(int)p_t::BORDER_BR] = '/';
        charContainer[(int)p_t::DATUM] = 'o';
        charContainer[(int)p_t::AXIS_HM] = '-';
        charContainer[(int)p_t::AXIS_HL] = '_';
        charContainer[(int)p_t::AXIS_V] = '|';
    }
    
    void setChar(p_t type, char c)
    {
        charContainer[(int)type] = c;
    }
    
    char getChar(p_t type)
    {
        return charContainer[(int)type];
    }
    
};
    
#endif

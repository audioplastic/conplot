//
//  DataSeries.h
//  conplot
//
//  Created by Nick on 15/02/2013.
//  Copyright (c) 2013 Nick. All rights reserved.
//

#ifndef conplot_DataSeries_hpp
#define conplot_DataSeries_hpp

#include <vector>
#include <iostream>


using namespace std;

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




#endif

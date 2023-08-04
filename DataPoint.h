//
// Created by Benjamin Quinto on 8/2/23.
//
#include <iostream>

struct DataPoint{
    std::pair<double, double> coordinate;
    std::string date; // format: year-month-day
    double avgTemp;
    double avgUncertainty;

    DataPoint(std::pair<double, double> coords, std::string date, double temp, double uncertainty){
        this->coordinate = coords;
        this->date = date;
        avgTemp = temp;
        avgUncertainty = uncertainty;
    }
    void Display(){
        std::cout << "(" << coordinate.first << ", " << coordinate.second << ") " << date << ", " << avgUncertainty << ", " << avgTemp << std::endl;
    }
};


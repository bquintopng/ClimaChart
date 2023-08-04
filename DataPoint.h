//
// Created by Benjamin Quinto on 8/2/23.
//
#include <iostream>
/*
struct DataPoint{
    std::string date; // format: year-month-day
    double avgTemp;
    double avgUncertainty;

    DataPoint(std::string date, double avgTemp, double avgUncertainty){
        this->date=date;
        this->avgTemp = avgTemp;
        this->avgUncertainty = avgUncertainty;
    }
    void Display(){
        std::cout<<date<<" | temp="<<avgTemp<<" ("<<avgUncertainty<<")";
    }
};

struct City{
    std::string name;
    std::pair<double, double> coordinates;
    std::vector<DataPoint> record;

    void Display(){
        std::cout<<name<< " ("<<coordinates.first<<", "<<coordinates.second<<")"<<std::endl;
        for(DataPoint& dp : record){
            std::cout<<"\t";
            dp.Display();
        }
    }
    double calcTempChange(int startyear, int endyear){
    }
}*/

struct DataPoint{
    std::pair<double, double> coordinate;
    std::string date; // format: year-month-day
    double avgTemp;
    double avgUncertainty;

    DataPoint(std::pair<double, double> coordinate, std::string date, double avgTemp, double avgUncertainty){
        this->coordinate = coordinate;
        this->date=date;
        this->avgTemp = avgTemp;
        this->avgUncertainty = avgUncertainty;
    }

    void Display(){
        std::cout << "(" << coordinate.first << ", " << coordinate.second << ") " << date << ", " << avgUncertainty << ", " << avgTemp << std::endl;
    }
};


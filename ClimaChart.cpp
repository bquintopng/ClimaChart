//
// Created by Benjamin Quinto on 8/4/23.
//

#include "ClimaChart.h"
double ClimaChart::convertCoord(std::string c){

    if(!c.empty()){
        double coord = stod(c);
        char cardinal = c.back();

        if(cardinal == 'S' || cardinal == 'W')
            coord = -coord;

        return coord;
    }else return 0.0;
}

void ClimaChart::calculateTempChange(int sortchoice){
    auto start_time = std::chrono::high_resolution_clock::now();
    std::map<std::pair<double,double>, std::vector<DataPoint>> record;
    for(auto& dp : data){
        record[dp.coordinate].push_back(dp);
    }

    std::vector<std::pair<std::pair<double,double>, double>> avgTempChange;
    for (const auto& entry : record) {
        const std::vector<DataPoint>& locationData = entry.second;

        // Sort the location data by year in ascending order
        std::vector<DataPoint> sortedData = locationData;
        if(sortchoice == 1){
            mergeSortYear(sortedData, 0, sortedData.size()-1);
        }else if(sortchoice ==2){
            quickSortByTime(sortedData, 0, sortedData.size()-1);
        }

        // Calculate temperature change and rate of change for the location
        if (sortedData.size() > 1) {
            double tempChange = sortedData.back().avgTemp - sortedData.front().avgTemp;

            int yearChange = stoi(sortedData.back().date.substr(0,4)) - stoi(sortedData.front().date.substr(0,4));

            // Calculate the temperature rate of change per year
            double tempRateOfChange = abs(tempChange / static_cast<double>(yearChange));

            // Store the result as a pair of location and temperature rate of change
            avgTempChange.push_back({entry.first, tempRateOfChange});
        }

    }

    for(auto& dp : avgTempChange){
        latitude.push_back(dp.first.first);
        longitude.push_back(dp.first.second);
        weight.push_back(dp.second);
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    long runtime = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    std::cout<<"Time to calculate w/ Merge Sort: "<< runtime <<" ms" <<std::endl;
}

void ClimaChart::calculateLinearRegression(int sortchoice){
    auto start_time = std::chrono::high_resolution_clock::now();
    std::map<std::pair<double,double>, std::vector<DataPoint>> record;
    for(auto& dp : data){
        record[dp.coordinate].push_back(dp);
    }

    std::vector<std::pair<std::pair<double,double>, double>> avgTempChange;
    for (const auto& entry : record) {
        const std::vector<DataPoint>& locationData = entry.second;

        // Sort the location data by year in ascending order
        std::vector<DataPoint> sortedData = locationData;
        if(sortchoice == 1){
            mergeSortYear(sortedData, 0, sortedData.size()-1);
        }else if(sortchoice ==2){
            quickSortByTime(sortedData, 0, sortedData.size());
        }

        // Calculate temperature change using linear regression
        if (sortedData.size() > 1) {
            int n = sortedData.size();
            double sum_x = 0.0;
            double sum_y = 0.0;
            double sum_xy = 0.0;
            double sum_xx = 0.0;
            double rateOfChange = 0.0;

            for (const auto& dp : sortedData) {
                int year = stoi(dp.date.substr(0,4));
                sum_x += year;
                sum_y += dp.avgTemp;
                sum_xy += year * dp.avgTemp;
                sum_xx += year * year;
            }

            rateOfChange = (n * sum_xy - sum_x * sum_y) / (n * sum_xx - sum_x * sum_x);

            // Store the result as a pair of location and temperature rate of change
            avgTempChange.push_back({entry.first, rateOfChange});
        }

    }

    for(auto& dp : avgTempChange){
        latitude.push_back(dp.first.first);
        longitude.push_back(dp.first.second);
        weight.push_back(dp.second);
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    long runtime = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    std::cout<<"Time to calculate w/ Merge Sort: "<< runtime <<" ms" <<std::endl;
}

void ClimaChart::readFile(std::string filename){

    auto start_time = std::chrono::high_resolution_clock::now();
    std::ifstream inputFile(filename);

    // Line Format:
    // Date, Avg Temp, Uncertainty, City, Country, latitude(NS), longitude(EW)
    std::string line = "";
    getline(inputFile, line);
    while(getline(inputFile, line)){
        std::string date;
        double temperature = 0.0;
        double uncertainty = 0.0;
        std::string cityname;
        double lat;
        double lon;
        std::string tempString;
        std::stringstream ss(line);


        // Date
        getline(ss, date, ',');
        // Temperature
        getline(ss, tempString, ',');
        if(!tempString.empty()) temperature = stod(tempString);
        // Uncertainty
        getline(ss, tempString, ',');
        if(!tempString.empty()) uncertainty = stod(tempString);

        // Country and City:
        getline(ss, cityname, ',');
        getline(ss, tempString, ',');

        // Latitude & Longitude
        getline(ss, tempString, ',');
        lat = convertCoord(tempString);
        getline(ss, tempString, '\r');
        lon = convertCoord(tempString);
        std::pair<double, double> coordinate = {lat, lon}; // latitude always precedes longitude

        DataPoint dp(coordinate, date, temperature, uncertainty);
        data.push_back(dp);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    long runtime = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    std::cout<<"Time to read "<<data.size()<<" entries: "<< runtime <<" ms" <<std::endl;
}

void ClimaChart::merge(std::vector<DataPoint>& data, int left, int middle, int right) {

    int leftArray = middle - left + 1;
    int rightArray = right - middle;
    std::vector<DataPoint> X;
    std::vector<DataPoint> Y;

    for(int i = 0; i < leftArray; i++) {
        X.push_back(data[left+i]);
    }
    for(int j = 0; j < rightArray; j++) {
        Y.push_back(data[middle+1+j]);
    }
    int leftP, rightP, mergedP;
    leftP = 0;
    rightP = 0;
    mergedP = left;

    while(leftP < leftArray && rightP < rightArray) {
        if(X[leftP].avgTemp <= Y[rightP].avgTemp) {
            data[mergedP] = X[leftP];
            leftP++;
        } else {
            data[mergedP] = Y[rightP];
            rightP++;
        }
        mergedP++;
    }
    while(leftP < leftArray) {
        data[mergedP] = X[leftP];
        leftP++;
        mergedP++;
    }
    while(rightP < rightArray) {
        data[mergedP] = Y[rightP];
        rightP++;
        mergedP++;
    }
}

int ClimaChart::partition(std::vector<DataPoint>& data, int low, int high) {
    double pivot = data[high].avgTemp;
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (data[j].avgTemp <= pivot) {
            i++;
            std::swap(data[i], data[j]);
        }
    }
    std::swap(data[i + 1], data[high]);
    return i + 1;
}

void ClimaChart::mergeSortCities(std::vector<DataPoint>& data, int left, int right) {
    if (left < right) {
        int middle = (left + right) / 2;
        mergeSortCities(data, left, middle);
        mergeSortCities(data, middle+1, right);

        merge(data, left, middle, right);

    }
}
void ClimaChart::quickSort(std::vector<DataPoint> &data, int low, int high)
{
    if (low < high)
    {
        int pi = partition(data, low, high);
        quickSort(data, low, pi - 1);
        quickSort(data, pi + 1, high);
    }
}

void ClimaChart::mergeYear(std::vector<DataPoint>& data, int left, int middle, int right) {

    int leftArray = middle - left + 1;
    int rightArray = right - middle;
    std::vector<DataPoint> L;
    std::vector<DataPoint> R;

    for(int i = 0; i < leftArray; i++) {
        L.push_back(data[left+i]);
    }
    for(int j = 0; j < rightArray; j++) {
        R.push_back(data[middle+1+j]);
    }
    int leftP, rightP, mergedP;
    leftP = 0;
    rightP = 0;
    mergedP = left;

    while(leftP < leftArray && rightP < rightArray) {
        if(L[leftP].date <= R[rightP].date) {
            data[mergedP] = L[leftP];
            leftP++;
        } else {
            data[mergedP] = R[rightP];
            rightP++;
        }
        mergedP++;
    }
    while(leftP < leftArray) {
        data[mergedP++] = L[leftP++];

    }
    while(rightP < rightArray) {
        data[mergedP++] = R[rightP++];
    }
}

void ClimaChart::mergeSortYear(std::vector<DataPoint>& data, int left, int right) {
    if (left < right) {

        int middle = (left + right) / 2;
        mergeSortYear(data, left, middle);
        mergeSortYear(data, middle+1, right);

        mergeYear(data, left, middle, right);

    }
}

int ClimaChart::partitionByTime(std::vector<DataPoint>& data, int low, int high) {
    std::string pivot = data[high].date; // Sort by date
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (data[j].date <= pivot) {
            i++;
            std::swap(data[i], data[j]);
        }
    }
    std::swap(data[i + 1], data[high]);
    return i + 1;
}

void ClimaChart::quickSortByTime(std::vector<DataPoint>& data, int low, int high) {
    if (low < high) {
        int pi = partitionByTime(data, low, high);
        quickSortByTime(data, low, pi - 1);
        quickSortByTime(data, pi + 1, high);
    }
}
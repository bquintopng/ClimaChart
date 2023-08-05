#include "ClimaChart.h"
using namespace std;
int main(){
    cout << "WELCOME TO CLIMACHART" << endl;
    cout << "Choose File, Sort, and Calculation type:"<<endl;
    cout << "----------------------------------------" << endl;
    ClimaChart chart;

    bool stop = false;

    while(!stop){
        string filename;
        int sort_type;

        cout<<"\nFile\t\t\tSort\t\tCalculation"
              "\n1. Major Cities\t1. Merge\t1. Rate of Change"
              "\n2. All Cities\t2. Quick\t2. Linear Regression"
              "\n3. Exit"
              "\n\n(Format: ie 000)"<<endl;

        string choice;
        cin>>choice;

        if(choice == "3"){
            break;
        }else if(choice.empty()){
            cout<<"Error: Invalid";
            return 0;
        }

        if(choice[0] == '1')
            filename = "archive/GlobalLandTemperaturesByMajorCity.csv";
        else if (choice[0] == '2')
            filename = "archive/GlobalLandTemperaturesByCity.csv";
        else{
            cout<<"Error: Invalid";
            break;
        }

        if(choice[1] == '1')
            sort_type = 1;
        else if (choice[1] == '2')
            sort_type = 2;
        else{
            cout << "Error: Invalid";
            break;
        }

        if(choice[2] == '1'){
            chart.readFile(filename);
            chart.calculateTempChange(sort_type);
        }
        else if(choice[2] == '2'){
            chart.readFile(filename);
            chart.calculateLinearRegression(sort_type);
        }
        else{
            cout << "Error: Invalid";
            break;
        }

        HeatMap graph(chart.latitude, chart.longitude, chart.weight);
        graph.Display();
        chart.reset();
    }

    return 0;
}


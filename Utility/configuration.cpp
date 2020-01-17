#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <math.h>
#include <QCoreApplication>
#include <QDir>

#include "ext.h"
using namespace std;

void Read_Config(){
    QString szPath = QCoreApplication::applicationDirPath() + "/adeSIL_Config";
    std::ifstream file(szPath.toStdString().c_str());
    std::string str;
    std::string file_contents[10];

    int n = 0;
    while (std::getline(file, str))
    {
        // --Skip empty lines.
        if (str.empty()) continue;
        // --Remove Leading White Space
        /*std::stringstream ss(str);
        ss >> std::ws;
        getline(ss, str);*/

        // --Remove all WhiteSpace from each line (Only for C++11 with []Lambda)
        str.erase(std::remove_if(str.begin(), str.end(),
                                 [](char &c) {
            return std::isspace<char>(c, std::locale::classic());
        }),
                  str.end());
        // --Skip Comment line
        std::string::size_type check = str.find("//");
        if (check != std::string::npos)
            continue;
        file_contents[n] += str;
        n++;
    }

    //    return;
    cout << "Configuration Setting :" << endl;
    cout << "----------------------------"<<endl;
    if(n==0){
        cout << "[Error!]: While Reading Configuration Setting , probaly due to file not exist." <<endl;
        cout <<"[Use Default Setting]" <<endl;
        cout <<"ch_I : AIN"<<ch_I<<endl;
        cout <<"ch_Q : AIN"<<ch_Q<<endl;
    }else
        for(int i  = 0; i <n; i++)
        {
            std::istringstream is_line(file_contents[i]);
            std::string key;
            if( std::getline(is_line, key, '=') )
            {
                std::string value;
                if( std::getline(is_line, value) ){
                    //                cout << key <<","<<value<<endl;
                   if(key == string("PGA")){
                        try {
                            int x = stoi(value);
                            if(x <0 || x>5)
                                x = 4;
                            cout<<"ADC PGA = ";
                            ADC_FSR = x;
                            switch(x){
                            case 0:
                            case 1:
                            case 2:
                            case 3:
                            case 4:
                                ADC_PGA = 0.256*pow(2,x);
                                break;
                            case 5:
                                ADC_PGA = 6.144;
                                break;
                            }
                            cout << ADC_PGA <<endl;
                        }
                        catch(std::invalid_argument& e){
                            // if no conversion could be performed
                            cout << "Error Reading ADC_PGA (use default:"<<ADC_PGA <<")"  <<endl;
                        }
                    }else if(key == string("ch_I")){
                        try {
                            int x = stoi(value);
                            if(x <6 && x >=0){
                                ch_I = x;
                                cout<<"ch_I= AIN"<<ch_I<<endl;
                            }else
                                cout<<"channel I Out of Bound(0~3)" <<endl;
                        }
                        catch(std::invalid_argument& e){
                            // if no conversion could be performed
                            cout << "Error Reading channel I(use default:"<<ch_I <<")"  <<endl;
                        }
                    }else if(key == string("ch_Q")){
                        try {
                            int x = stoi(value);
                            if(x <6 && x >=0){
                                ch_Q = x;
                                cout<<"ch_Q= AIN"<<ch_Q<<endl;
                            }else
                                cout<<"channel Q Out of Bound(0~3)" <<endl;
                        }
                        catch(std::invalid_argument& e){
                            // if no conversion could be performed
                            cout << "Error Reading channel Q(use default:"<<ch_Q <<")" <<endl;
                        }
                    }else if(key == string("GUI")){
                        try {
                            int x = stoi(value);
                            if(x==0){
                                GUI_Switch = 0;
                                cout<<"GUI : Off"<<endl;
                            }else{
                                GUI_Switch = abs(x);
                                cout<<"GUI : On"<<endl;
                            }
                        }
                        catch(std::invalid_argument& e){
                            // if no conversion could be performed
                            cout << "Error Reading GUI_Setting(use default: On)" <<endl;
                        }
                    }

                }
            }
        }
    cout << "----------------------------"<<endl;
}

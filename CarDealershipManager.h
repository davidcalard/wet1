//
// Created by omerg on 05/05/2021.
//


#ifndef WET1_CARDEALERSHIPMANAGER_H
#define WET1_CARDEALERSHIPMANAGER_H

#include "library.h"
#include "AVLTree.h"
#include "ModelGrades.h"


typedef int CarID,index,Sales_num,Grade;
class Car{
public:
    int models_num; //num of models
    Sales_num* sales;//num of sales of each model
    ModelGrades* models;
    std::pair<index,Sales_num> best_m_seller = std::make_pair(0,0);
    Car(int n, CarID carId):models_num(n){
        sales = new int[n];
        ///need to init models
        for(int i=0;i<n;i++)models[i]=ModelGrades(carId,i);
        for(int i=0;i<n;i++)    sales[i]=0;
        best_m_seller;
    }
    ~Car(){delete[] sales;delete[] models;}

    bool updateSales(int model){
        if(model>=models_num)return false;
        else sales[model]++;
        return true;
    }
};




class CarDealershipManager {
public:
    AVLTree<ModelGrades,ModelGrades> models_grades;
    std::pair<CarID ,Sales_num> best_c_seller = std::make_pair(0,0);
    AVLTree<CarID,Car> cars;
    AVLTree<CarID,AVLTree<ModelGrades,ModelGrades>> zero_models;
};

#endif
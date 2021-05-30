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
        models = new ModelGrades[n];
        for(int i=0;i<n;i++) {
            models[i]=ModelGrades(carId,i);
            sales[i]=0;
        }
    }
    Car(Car& to_copy){
        this->models_num = to_copy.models_num;
        this->best_m_seller = to_copy.best_m_seller;
        sales = new int[models_num];
        models = new ModelGrades[models_num];
        for(int i=0;i<models_num;i++) {
            this->models[i]=to_copy.models[i];
            sales[i]=to_copy.sales[i];
        }
    }
    ~Car() {delete[] sales;delete[] models;}

    bool updateSales(int model){
        if(model>=models_num)return false;
        sales[model]++;
        models[model].grade+=10;
        return true;
    }
};




class CarDealershipManager {
public:
    AVLTree<ModelGrades,ModelGrades>* models_grades;
    std::pair<CarID ,Sales_num> best_c_seller = std::make_pair(0,0);
    AVLTree<CarID,Car>* cars;
    AVLTree<CarID,AVLTree<ModelGrades,ModelGrades>>* zero_models;

    CarDealershipManager():models_grades(new AVLTree<ModelGrades,ModelGrades>()),
    cars(new AVLTree<CarID,Car>()),zero_models(new AVLTree<CarID,AVLTree<ModelGrades,ModelGrades>>()){};
    ~CarDealershipManager(){
        if(models_grades) delete models_grades;
        if(cars) delete cars;
        if(zero_models) delete zero_models;
    }

    void* CDMInit();

    StatusType CDMAddCarType(void *DS, int typeID, int numOfModels);

    StatusType CDMRemoveCarType(void *DS, int typeID);

    StatusType CDMSellCar(void *DS, int typeID, int modelID);

    StatusType CDMMakeComplaint(void *DS, int typeID, int modelID, int t);

    StatusType CDMGetBestSellerModelByType(void *DS, int typeID, int * modelID);

    StatusType CDMGetWorstModels(void *DS, int numOfModels, int *types, int *models);

    void CDMQuit(void** DS);
};

#endif
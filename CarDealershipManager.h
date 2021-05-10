//
// Created by omerg on 05/05/2021.
//

#ifndef WET1_CARDEALERSHIPMANAGER_H
#define WET1_CARDEALERSHIPMANAGER_H

#include "library.h"
#include "AVLTree.h"
#include "ModelGrades.h"

typedef int CarID,index,Sales_num;
class Car{
    Sales_num* sales;			//num of sales of each model
    int models_num;		//num of models
    std::pair<index,Sales_num> best_m_seller;
public:
    Car(int n):models_num(n){
        sales = new int[n];
    }
    ~Car(){delete[] sales;}
};




class CarDealershipManager {
    AVLTree<CarID,Car> cars;
    AVLTree<ModelGrades,ModelGrades> models_grades;
    std::pair<int,int> best_c_seller;


    void * Init();
    StatusType AddCarType (void *DS, int typeID, int numOfModels);

};

StatusType CarDealershipManager::AddCarType(void *DS, int typeID, int numOfModels){
    if(!DS||typeID>0||) return FAILURE;         //bad argument
    CarDealershipManager *ourDS = static_cast<CarDealershipManager*>(DS);
    Car new_car(numOfModels);
    ourDS->cars.insertNode(typeID,new_car);
}

#endif //WET1_CARDEALERSHIPMANAGER_H

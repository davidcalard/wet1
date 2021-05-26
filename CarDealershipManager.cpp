//
// Created by omerg on 05/05/2021.
//

#include "CarDealershipManager.h"
#include "library.h"

void* Init(){
    return (void*)(new CarDealershipManager());
}

StatusType AddCarType(void *DS, int typeID, int numOfModels){
    if(!DS||typeID<=0||numOfModels<=0) return FAILURE;         //bad argument
    CarDealershipManager *ourDS = static_cast<CarDealershipManager*>(DS);
    Car new_car(numOfModels);
    ourDS->cars.insertNode(typeID,new_car);
    return SUCCESS;
}

StatusType RemoveCarType(void *DS, int typeID){
    if(!DS||typeID<=0) return INVALID_INPUT;
    auto* CDM=(CarDealershipManager*)DS;
    if(CDM->cars.findNode(typeID)== nullptr)return FAILURE;
    CDM->cars.removeNode(typeID);
    return SUCCESS;
}


StatusType MakeComplaint(void *DS, int typeID, int modelID, int t){
    if(!DS||typeID<=0||modelID<0) return INVALID_INPUT;
    auto* CDM=(CarDealershipManager*)DS;
    Car* car=&CDM->cars.findNode(typeID)->data;
    if(car== nullptr)return FAILURE;
    if(car->models_num<modelID)return INVALID_INPUT;
    int oldGrade=car->sales[modelID];
    car->sales[modelID]-= 100/t;
    int NewGrade=car->sales[modelID];
    CDM->models_grades.findNode(oldGrade)->data.removeNode(ModelGrades(typeID,oldGrade));
    AVLNode<int,AVLTree<ModelGrades,ModelGrades>>* grade=CDM->models_grades.findNode(NewGrade);
    if(grade!= nullptr) grade->data.insertNode
                (ModelGrades(typeID,modelID),ModelGrades(typeID,modelID));
    else {
        CDM->models_grades.insertNode(NewGrade,AVLTree<ModelGrades,ModelGrades>());
        CDM->models_grades.findNode(NewGrade)->data.insertNode
                (ModelGrades(typeID,modelID),ModelGrades(typeID,modelID));
    }
    return SUCCESS;
}

StatusType SellCar(void *DS, int typeID, int modelID){
    if(!DS||typeID<=0||modelID<0)   return INVALID_INPUT;
    auto* ourDS = static_cast<CarDealershipManager*>(DS);
    AVLNode<CarID,Car> *car_to_sell = ourDS->cars.findNode(typeID);
    if(!car_to_sell) return FAILURE;
    if(!car_to_sell->data.updateSales(modelID))    return INVALID_INPUT;//modelID > numOfModels
    if(car_to_sell->data.best_m_seller.second == car_to_sell->data.sales[modelID])// update best_m_seller if needed
        if(modelID < car_to_sell->data.best_m_seller.first) {
            car_to_sell->data.best_m_seller.first = modelID;
            car_to_sell->data.best_m_seller.second = car_to_sell->data.sales[modelID];
        }
    if(ourDS->best_c_seller.second == car_to_sell->data.sales[modelID])
        if(typeID < ourDS->best_c_seller.first){
            ourDS->best_c_seller.first = typeID;
            ourDS->best_c_seller.second = car_to_sell->data.sales[modelID];
        }
    return SUCCESS;
}

StatusType GetBestSellerModelByType(void *DS, int typeID, int * modelID){

    if(DS==nullptr || typeID<0 || modelID==nullptr)return INVALID_INPUT;
    auto* CDM=(CarDealershipManager*)DS;
    if(typeID==0){
        if(CDM->best_c_seller == std::make_pair(0,0))   return FAILURE; //empty system
        *modelID= CDM->best_c_seller.first;
        return SUCCESS;
    }
    auto our_car = CDM->cars.findNode(typeID);
    if(our_car == nullptr)return FAILURE; // typeID car not in system
    *modelID = our_car->data.best_m_seller.first;
    return SUCCESS;
}

StatusType GetWorstModels(void *DS, int numOfModels , int *types, int *models){
    if(DS== nullptr || numOfModels<=0)return INVALID_INPUT;
    auto* CDM=(CarDealershipManager*)DS;
    auto father = CDM->models_grades.min;
    int cycles=numOfModels;

    do
    {
        auto innerMin= father->data.min;
        father->data.smallinOrder(innerMin,types,models,&cycles);  /*inner tree*/
        if(father->right != nullptr && cycles!=0){/*bigger tree*/
            CDM->models_grades.biginOrder(father->right,types,models,&cycles);
        }
        father= father->parent;
    }while(father!= nullptr && cycles!= 0);

    return SUCCESS;
}

void Quit(void **DS){
    auto* CDM=(CarDealershipManager**)DS;
    delete CDM;
}
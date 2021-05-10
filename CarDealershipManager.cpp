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
    /*
    if(DS== nullptr || typeID<0)return INVALID_INPUT;
    auto* CDM=(CarDealershipManager*)DS;
    if(typeID==0){
        *modelID= CDM->getBestSellerTypeId();
        return SUCCESS;
    }
     */
    return SUCCESS;
    ///car = CDM.CArsTree.find(key=typeID);
    ///if(car==null)return fail
    ///*modelID=car.best;
    ///SUCCESS
}

StatusType GetWorstModels(void *DS, int numOfModels , int *types, int *models){
    /*if(DS== nullptr || numOfModels<=0)return INVALID_INPUT;
    auto* CDM=(CarDealershipManager*)DS;
    auto father = CDM->models_grades.min;
    do
    father->data.min   /*inner tree*/
    return SUCCESS;

///if(father.right.preOrder()) /*bigger tree*/
///father= father.father;
///while(father!= nullptr)

}

void Quit(void **DS){}
//
// Created by omerg on 05/05/2021.
//

#include "CarDealershipManager.h"
#include "library.h"

void*  CarDealershipManager::CDMInit(){
    return (void*)(new CarDealershipManager());
}

StatusType CarDealershipManager::CDMAddCarType(void *DS, int typeID, int numOfModels){
    if(!DS || typeID<=0 || numOfModels<=0) return INVALID_INPUT;         //bad argument
    auto ourDS = (CarDealershipManager*)DS;
    if(ourDS->cars->findNode(typeID)!= nullptr)return FAILURE;
    Car new_car(numOfModels,typeID);
    ourDS->cars->insertNode(typeID,new_car);
    for(int i=0;i<numOfModels;i++){
        ourDS->models_grades->insertNode(new_car.models[i],new_car.models[i]);
    }
    ///zero tree for later...
    /*
    auto zero_tree= AVLTree<ModelGrades,ModelGrades>(ourDS->models_grades.min->data.completeTree(numOfModels));
    zero_tree.deleteForComplete()
    ourDS->zero_models.insertNode(typeID,zero_tree);
    */
    return SUCCESS;
}

StatusType CarDealershipManager::CDMRemoveCarType(void *DS, int typeID){
    if(!DS||typeID<=0) return INVALID_INPUT;
    auto* CDM=(CarDealershipManager*)DS;
    auto car=CDM->cars->findNode(typeID);
    if(CDM->cars->findNode(typeID)== nullptr)return FAILURE;
    for(int i=0;i<car->data.models_num;i++){
        CDM->models_grades->removeNode(car->data.models[i]);
    }
    CDM->cars->removeNode(typeID);
    return SUCCESS;
}


StatusType CarDealershipManager::CDMMakeComplaint(void *DS, int typeID, int modelID, int t){
    if(!DS||typeID<=0||modelID<0) return INVALID_INPUT;
    auto CDM=(CarDealershipManager*)DS;
    auto car_n= CDM->cars->findNode(typeID);
    if(car_n== nullptr)return FAILURE;
    auto car=&car_n->data;
    if(car->models_num<modelID)return FAILURE;
    auto oldGrade=car->models[modelID];
    car->models[modelID].grade-= 100/t;
    auto NewGrade=car->models[modelID];
    CDM->models_grades->removeNode(oldGrade);
    CDM->models_grades->insertNode(NewGrade,NewGrade);
    return SUCCESS;
}

StatusType CarDealershipManager::CDMSellCar(void *DS, int typeID, int modelID){
    if(!DS||typeID<=0||modelID<0)   return INVALID_INPUT;
    auto* ourDS = static_cast<CarDealershipManager*>(DS);
    AVLNode<CarID,Car> *car_to_sell = ourDS->cars->findNode(typeID);
    if(!car_to_sell) return FAILURE;
    if(!car_to_sell->data.updateSales(modelID))    return FAILURE;//modelID > numOfModels

    if(car_to_sell->data.best_m_seller.second == car_to_sell->data.sales[modelID])// update best_m_seller if needed
        if(modelID < car_to_sell->data.best_m_seller.first){
            car_to_sell->data.best_m_seller.first = modelID;
            car_to_sell->data.best_m_seller.second = car_to_sell->data.sales[modelID];
        }
    if((car_to_sell->data.best_m_seller.second < car_to_sell->data.sales[modelID])){
        car_to_sell->data.best_m_seller.first = modelID;
        car_to_sell->data.best_m_seller.second = car_to_sell->data.sales[modelID];
    }
    if(ourDS->best_c_seller.second == car_to_sell->data.sales[modelID])
        if(typeID < ourDS->best_c_seller.first){
            ourDS->best_c_seller.first = typeID;
            ourDS->best_c_seller.second = car_to_sell->data.sales[modelID];
        }
    if(ourDS->best_c_seller.second < car_to_sell->data.sales[modelID]){
        ourDS->best_c_seller.first = typeID;
        ourDS->best_c_seller.second = car_to_sell->data.sales[modelID];
    }
    return SUCCESS;
}

StatusType CarDealershipManager::CDMGetBestSellerModelByType(void *DS, int typeID, int * modelID){

    if(DS==nullptr || typeID<0 || modelID==nullptr)return INVALID_INPUT;
    auto* CDM=(CarDealershipManager*)DS;
    if(typeID==0){
        if(CDM->best_c_seller == std::make_pair(0,0))   return FAILURE; //empty system
        *modelID= CDM->best_c_seller.first;
        return SUCCESS;
    }
    auto our_car = CDM->cars->findNode(typeID);
    if(our_car == nullptr)return FAILURE; // typeID car not in system
    *modelID = our_car->data.best_m_seller.first;
    return SUCCESS;
}

void helper(AVLNode<ModelGrades,ModelGrades>* node,int *types, int *models,int* i){
    types[*i]=node->data.carID;
    models[*i]=node->data.modelID;
    (*i)++;
}

StatusType CarDealershipManager::CDMGetWorstModels(void *DS, int numOfModels , int *types, int *models){
    if(DS== nullptr || numOfModels<=0)return INVALID_INPUT;
    auto* CDM=(CarDealershipManager*)DS;
    auto father = CDM->models_grades->min;
    if(father== nullptr)return INVALID_INPUT;
    int cycles=numOfModels, i=0;

    do
    {
        helper(father,types,models,&i);
        cycles--;
        if(father->right != nullptr && cycles!=0){/*bigger tree*/
            CDM->models_grades->inOrder(father->right,helper,types,models,&cycles, &i);
        }
        father= father->parent;
    }while(father!= nullptr && cycles!= 0);
    if(cycles!=0)return FAILURE;
    return SUCCESS;
}

void CarDealershipManager::CDMQuit(void **DS){
    auto CDM=(CarDealershipManager**)DS;
    delete *CDM;
    *DS=nullptr;
}
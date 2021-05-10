//
// Created by omerg on 05/05/2021.
//

#include "CarDealershipManager.h"
#include "library.h"

void* Init(){
    return (void*)(new CarDealershipManager());
}



StatusType GetBestSellerModelByType(void *DS, int typeID, int * modelID){
    if(DS== nullptr || typeID<0)return INVALID_INPUT;
    auto* CDM=(CarDealershipManager*)DS;
    if(typeID==0){
        *modelID= CDM->getBestSellerTypeId();
        return SUCCESS;
    }
    ///car = CDM.CArsTree.find(key=typeID);
    ///if(car==null)return fail
    ///*modelID=car.best;
    ///SUCCESS
}
StatusType GetWorstModels(void *DS, int numOfModels , int *types, int *models){
    if(DS== nullptr || numOfModels<=0)return INVALID_INPUT;
    auto* CDM=(CarDealershipManager*)DS;
    ///AVLNode<>* father = CDM.WorstModel;
    ///do
    ///father.models.preOrder()   /*inner tree*/

    ///if(father.right.preOrder()) /*bigger tree*/
    ///father= father.father;
    ///while(father!= nullptr)
}

//
// Created by omerg on 05/05/2021.
//

#include"library.h"
#include"CarDealershipManager.h"


void *Init() {
    auto *DS = new CarDealershipManager ();
    return (void*)DS;
}
StatusType AddCarType(void *DS, int typeID, int numOfModels) {
    return ((CarDealershipManager *)DS)->CDMAddCarType(DS,typeID, numOfModels);
}

StatusType RemoveCarType(void *DS, int typeID){
    return ((CarDealershipManager*)DS)->CDMRemoveCarType(DS,typeID);
}

StatusType SellCar(void *DS, int typeID, int modelID){
    return ((CarDealershipManager*)DS)->CDMSellCar(DS,typeID, modelID);
}

StatusType MakeComplaint(void *DS, int typeID, int modelID, int t){
    return ((CarDealershipManager*)DS)->CDMMakeComplaint(DS,typeID,modelID,t);
}

StatusType GetBestSellerModelByType(void *DS, int typeID, int * modelID){
    return ((CarDealershipManager*)DS)->CDMGetBestSellerModelByType(DS,typeID,modelID);
}

StatusType GetWorstModels(void *DS, int numOfModels, int *types, int *models){
    return ((CarDealershipManager*)DS)->CDMGetWorstModels(DS,numOfModels, types, models);
}

void Quit(void** DS){
    return ((CarDealershipManager*)DS)->CDMQuit(DS);
}
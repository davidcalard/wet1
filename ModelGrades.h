//
// Created by omerg on 05/05/2021.
//

#ifndef WET1_MODELGRADES_H
#define WET1_MODELGRADES_H


class ModelGrades {
    int grade;
    int carID;
    int modelID;

    ModelGrades(int carID, int modelID): carID(carID), modelID(modelID){}

};

bool operator> (ModelGrades a, ModelGrades b){
    if(a.grade > b.grade)return true;
    else if (a.grade < b.grade)return false;
    else if(a.carID > b.carID)return true;
    else if (a.carID < b.carID) return false;
    else return (a.modelID > b.modelID);
}
bool operator== (ModelGrades a, ModelGrades b){
}
}
bool operator< (ModelGrades a, ModelGrades b){
    return !(a>b);



#endif //WET1_MODELGRADES_H

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
    bool operator> (ModelGrades b) const{
        if(this->grade > b.grade)return true;
        else if (this->grade < b.grade)return false;
        else if(this->carID > b.carID)return true;
        else if (this->carID < b.carID) return false;
        else return (this->modelID > b.modelID);
    }
    bool operator== (ModelGrades b) const{
        return (this->grade==b.grade);
    }


    bool operator< (ModelGrades b) const {
        if(this->grade < b.grade)return true;
        else if (this->grade > b.grade)return false;
        else if(this->grade < b.carID)return true;
        else if (this->carID > b.carID) return false;
        else return (this->modelID < b.modelID);
    }

};





#endif //WET1_MODELGRADES_H

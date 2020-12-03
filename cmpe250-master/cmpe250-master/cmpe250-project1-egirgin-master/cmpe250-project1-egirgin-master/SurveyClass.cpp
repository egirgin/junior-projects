#include "SurveyClass.h"

SurveyClass::SurveyClass() {
    this->members= new LinkedList();
}

SurveyClass::SurveyClass(const SurveyClass &other) {
    this->members= new LinkedList(*(other.members));
}

SurveyClass &SurveyClass::operator=(const SurveyClass &list) {
    this->members= new LinkedList(*(list.members));
    return *this;
}

SurveyClass::SurveyClass(SurveyClass &&other) {
    this->members= new LinkedList();
    *this->members= move(*other.members);
    other.members= nullptr;
}

SurveyClass &SurveyClass::operator=(SurveyClass &&list) {
    this->members= new LinkedList();
    *this->members=move(*list.members);
    list.members= nullptr;
    return *this;
}

SurveyClass::~SurveyClass() {
    delete this->members;
    this->members= nullptr;
}

float round(float var)
{
    // 37.66666 * 100 =3766.66
    // 3766.66 + .5 =37.6716    for rounding off value
    // then type cast to int so value is 3766
    // then divided by 100 so the value converted into 37.66
    float value = (int)(var * 100);
    return (float)value / 100;
}

void SurveyClass::handleNewRecord(string _name, float _amount) {

    if(this->members->length==0){
        this->members->pushTail(_name,_amount);
    }
    else{
        Node *current = this->members->head;
        for(int i=0;i<this->members->length && current!= nullptr;i++){
            //cout<<"Current Name: "<<current->name<<" target Name: "<<_name<<endl;
            if(current->name==_name){
                current->amount=_amount;
                break;
            }
            current=current->next;
        }
        if(current== nullptr){
            this->members->pushTail(_name,_amount);
        }
    }
}

float SurveyClass::calculateMinimumExpense() {
    Node *current = this->members->head;
    float min=current->amount;
    for(int i=0;i<this->members->length;i++){
        if(current->amount<min){
            min=current->amount;
        }
        else{
            current=current->next;
        }
    }
    min= round(min);
    return min;
}

float SurveyClass::calculateMaximumExpense() {
    Node *current = this->members->head;
    float max=current->amount;
    for(int i=0;i<this->members->length;i++){
        if(current->amount>max){
            max=current->amount;
        }
        else{
            current=current->next;
        }
    }
    max= round(max);
    return max;
}

float SurveyClass::calculateAverageExpense() {
    Node *current = this->members->head;
    float sum=0;
    for(int i=0;i<this->members->length;i++){
        sum+=current->amount;
        current=current->next;
    }
    sum=round(sum);
    return sum/this->members->length;
}

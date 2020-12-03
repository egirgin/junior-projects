#include "LinkedList.h"
#include <iostream>

using namespace std;

LinkedList::LinkedList() {
    this->head= nullptr;
    this->tail= nullptr;
    this->length= 0;
}


//copy constructor
LinkedList::LinkedList(const LinkedList &list) {
    //cout<<&list<<" listesi "<<this<<" listesine kopyalanıyor."<<endl;
    this->head = new Node(*(list.head));
    this->length=list.length;

    Node *current = this->head;
    while(current->next){
        current=current->next;
    }
    this->tail=current;
}

//copy constructor operator
LinkedList& LinkedList::operator=(const LinkedList &list) {
    //cout<<&list<<" listesi "<<this<<" listesine assign edilerek kopyalanıyor."<<endl;

    this->head=new Node(*(list.head));
    this->length=list.length;

    Node *current = this->head;
    while(current->next){
        current=current->next;
    }
    this->tail=current;

    return *this;

}

//move constructor
LinkedList::LinkedList(LinkedList &&list) {
    //cout<<&list<<" listesi "<<this<<" listesine move ediliyor"<<endl;
    this->head = move(list.head);
    this->tail = move(list.tail);
    this->length = move(list.length);
    
    list.head= nullptr;
    list.tail= nullptr;
    list.length=0;

    Node *track = this->head;
    while(track->next){
        track=track->next;
    }
    this->tail=track;

}
//move constructor operator
LinkedList& LinkedList::operator=(LinkedList &&list) {
    //cout<<&list<<" listesi "<<this<<" listesine operatör ile move ediliyor"<<endl;
    
    delete this->head;
    if(list.length == 0){
        this->head=nullptr;
        this->tail=nullptr;
        this->length=0;
    }
    else{
        this->head = move(list.head);
        this->tail = move(list.tail);
        this->length = move(list.length);

        list.head= nullptr;
        list.tail= nullptr;
        list.length=0;

        Node *current = this->head;
        while(current->next){
            current=current->next;
        }
        this->tail=current;
    }

    return *this;
}

//destructor
LinkedList::~LinkedList() {
    //cout<<this<<" listesi siliniyor"<<endl;
    delete head;
    head= nullptr;
}

float round(float var)
{
    // 37.66666 * 100 =3766.66
    // 3766.66 + .5 =37.6716    for rounding off value
    // then type cast to int so value is 3766
    // then divided by 100 so the value converted into 37.66
    float value = (int)(var * 100 + .5);
    return (float)value / 100;
}


void LinkedList::pushTail(string _name, float _amount) {
    //add one element and set it as head and tail if the list is empty

    //_amount= round(_amount);

    if(this->length==0){

        this->head=new Node(_name,_amount);
        this->tail=this->head;
        length++;
    }
    else {
        this->tail->next = new Node(_name, _amount);
        this->tail=this->tail->next;
        length++;
    }
    //cout<< this << " linkedlistine " <<(this->tail)<<" node'u eklendi" <<" uzunluğu: "<<this->length << " oldu"<<endl;
}
void LinkedList::updateNode(string _name, float _amount) {
    Node *current=this->head;
    for(int i=0;i<this->length;i++){
        if(current->name.compare(_name)==0){
            current->amount=_amount;
            //cout<<this<<" linkedlistinin "<<_name<<" elemanı "<<_amount<<" olarak güncellendi"<<"adres "<<current<<endl;
            break;
        }
        else{
            current=current->next;
        }
    }
}

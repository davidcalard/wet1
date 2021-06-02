//
// Created by omerg on 03/05/2020.
//



//===================================================================================
#ifndef WET1_ORDEREDLIST_H
#define WET1_ORDEREDLIST_H

#include <cstdlib>



class OrderedListNode {
public:
    int object;
    OrderedListNode*  prev;
    OrderedListNode* next;

    OrderedListNode(int object, OrderedListNode* prev= nullptr, OrderedListNode* next= nullptr):
             object(object),prev(prev),next(next){}
    OrderedListNode(const OrderedListNode& node):object(0),prev(nullptr),next(nullptr){}
    ~OrderedListNode(){
        prev= nullptr;
        next= nullptr;
    }



    friend bool operator==(const OrderedListNode& a, int b){
        return (a.object == b);
    }
    friend bool operator!=(const OrderedListNode& a, int b){
        return (a.object != b);
    }
    friend bool operator<(const OrderedListNode& a, const OrderedListNode& b){
        return a.object < b.object;
    }
    friend bool operator>(const OrderedListNode& a, const OrderedListNode& b){
        return a.object > b.object;
    }




};


class OrderedList{
public:
    OrderedListNode* start;
    OrderedListNode* end;
    int len;

    OrderedList(): start(nullptr),end(nullptr),len(0){}
    OrderedList(int n):start(nullptr),end(nullptr),len(n){
        if(n<=0)return;
        this->start= new OrderedListNode(0);

        //n>=1
        auto node = start;
        for (int i = 1; i < n; ++i) {
            node->next = new OrderedListNode(i,node);
            node=node->next;
        }
        this->end=node;
    }

    ~OrderedList() {
        destroyRecurs(this->start);
    }

    void destroyRecurs(OrderedListNode* strt){
        if(strt== nullptr)return;
        destroyRecurs(strt->next);
        delete strt;
    }
    void remove(OrderedListNode* node){
        if(node== nullptr)return;
        auto p=node->prev;
        auto n=node->next;
        if(n== nullptr)this->end=p;
        else n->prev=p;
        if(p== nullptr)this->start=n;
        else p->next=n;
        len--;
    }
    bool isEmpty(){return start == nullptr;}
};


#endif //WET1_ORDEREDLIST_H

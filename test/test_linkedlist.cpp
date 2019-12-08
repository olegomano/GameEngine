#include "../src/linkedlist.h"
#include <vector>
#include <list>
#include <string>
#include <iostream>

template<typename T>
void insert(T& t){
    for(int i = 0; i < 1<<22; i++){
        t.push_back(i);
    }
    while(t.size() > 0){
        t.pop_front();
        t.pop_back();
    }
}

template<typename T>
void insertTest(T& t){
    for(int i = 0; i < 10;i++){
        insert(t);
    }
}

template<typename T>
void iterateTest(T& t){
    for(int i = 0; i < 1 << 18; i++){
        for(int b = 0; b < 8; b++){
            t.push_front(i);
        }
        for(int b = 0; b < 8; b++){
            t.push_back(i);
        }
    }
    for(auto iter = t.begin(); iter != t.end(); ++iter){
        (*iter)++;
    }
}

int main(){
    LinkedList<__uint128_t> intList;
    std::list<__uint128_t>  intVector;
    //insertTest(intVector);
  //  insertTest(intList);
    iterateTest(intVector);
}
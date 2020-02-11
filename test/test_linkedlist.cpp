#include <vector>
#include <list>
#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <core.h>

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
            t.push_front(glm::vec4(0,0,0,0));
        }
        for(int b = 0; b < 8; b++){
            t.push_back(glm::vec4(0,0,0,0));
        }
    }
    for(auto iter = t.begin(); iter != t.end(); ++iter){
        (*iter)[0]++;
    }
}

int main(){
    LinkedList<glm::vec4> intList;
    //std::list<glm::vec4>  intVector;
    ///iterateTest(intList);
    //insertTest(intList);
    iterateTest(intList);
}

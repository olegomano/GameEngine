#include <iostream>
#include "../src/rmanager.h"
#include <string>
#include <glm/glm.hpp>

class TestClass{
public:
    int strLen;
    TestClass(const std::string& s){
        strLen = s.size();
    }
};

int main(){
    RManager<Resource<TestClass,std::string>> manager(128);
    auto a = manager.create("Hello");
    std::cout << a->strLen << " with arg " << a.params() << std::endl;
    return 1;
}
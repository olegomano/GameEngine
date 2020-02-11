extern "C"{
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}
#include <core.h>
#include <string.h>
#include <iostream>
#include <map>

struct TestStuct{
    int a;
    int b;
    int c;
    int d;

    TestStuct(int a, int b, int c, int d){}
    TestStuct(){}
};

template<>
int lua::push_struct(lua_State* lua,const TestStuct& t){
    return lua::LuaRef::TABLE;
}

int main(int argc,char** argv){
    std::string path(argv[1]);
    lua::Script s(path);
    if(!s.load()){
        std::cout << "Failed to load lua " << std::endl;    
        return 1;
    }
    
    return 0;
}
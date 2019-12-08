#include <iostream>
#include "../src/sdlcontext.cpp"

int main(){
    SDLContext<int,int,float> c;
    c.init(640,640);
    c.loopForever();
}
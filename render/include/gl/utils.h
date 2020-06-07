#ifndef _RENDERER_GL_UTILS_H_
#define _RENDERER_GL_UTILS_H_
#include <string>
#include <GL/glew.h>
#include <GL/gl.h>
/**
#define _CHECK_FUNCTION_(function){ \
template<typename T>\
struct has_#function {\
    template<typename U>\
    static constexpr decltype(std::declval<U>().#function(),bool())\
    test_#function(int){\
        return true;\
    };\     
    template<typename U>\
    static constexpr bool test_#function(...){\
        return false;\
    };\
    static constexpr bool value = test_#function<T>(int());\
};\
}
**/

static bool _check_gl_error(const std::string& prefix,std::string& out) {
        GLenum err (glGetError());
        bool res = false;
        while(err!=GL_NO_ERROR) {
                res = true;
                std::string error;
                
                switch(err) {
                        case GL_INVALID_OPERATION:      error="INVALID_OPERATION";      break;
                        case GL_INVALID_ENUM:           error="INVALID_ENUM";           break;
                        case GL_INVALID_VALUE:          error="INVALID_VALUE";          break;
                        case GL_OUT_OF_MEMORY:          error="OUT_OF_MEMORY";          break;
                        case GL_INVALID_FRAMEBUFFER_OPERATION:  error="INVALID_FRAMEBUFFER_OPERATION";  break;
                        default: error="UNKNOW_ERROR"; break;
                }
                out+= prefix + " " + error + "\n";                
                err=glGetError();
        }
        return res;
}
#endif

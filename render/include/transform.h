#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_
#include <glm/glm.hpp>
#include <iostream>

class Transform{
friend std::ostream& operator<<(std::ostream& out, const Transform& trans);
public:
    inline void rotate(const glm::vec4& axis, float angle);
    inline void rotate(float x, float y, float z, float angle);
    
    inline void set(const glm::mat4& m);
    inline void setPosition(const glm::vec4& v);
    inline void setPosition(const glm::vec3& v);
    void setPosition(float x, float y, float z);

    void scale(float xyz);
    inline void scale(float x, float y, float z);

    inline void lookAt(const glm::vec3& v);
    inline void lookAt(const glm::vec4& v);
    inline void lookAt(float x, float y, float z);
    
    inline void apply(const glm::mat4& in, glm::mat4& out);
    inline void undo(glm::mat4& out);

    inline glm::mat4 transform() const {return m_transform;}
    inline glm::mat4 inverse() const {return m_inverse;}

    Transform& operator = (const glm::mat4 mat);

private:
    glm::mat4 m_transform = glm::mat4(1);
    glm::mat4 m_inverse   = glm::mat4(1);
};

#endif

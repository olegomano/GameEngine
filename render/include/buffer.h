#ifndef _RENDER_BUFFER_H_
#define _RENDER_BUFFER_H_
#include <cinttypes>
namespace render{

class IBuffer{
public:
  virtual void create(uint32_t size) = 0;
  virtual void write(uint8_t* buffer, uint32_t count) = 0;
  virtual void write(uint8_t* buffer, uint32_t offset,uint32_t count) = 0;
  
  template<typename T>
  void createImm(const T& floats){
    float* data = new float[floats.size()];
    int count = 0;
    for(float f : floats){
      data[count++] = f;
    }
    create(floats.size() * sizeof(float));
    write((uint8_t*)data,floats.size() * sizeof(float));
    delete[] data;
  }

  template<typename T>
  void updateImm(const T& floats){
    float* data = new float[floats.size()];
    int count = 0;
    for(float f : floats){
      data[count++] = f;
    }
    write((uint8_t*)data,floats.size() * sizeof(float));
    delete[] data;
  }
};

class IAttribute{
public:
  virtual IBuffer& owner() = 0;
  virtual uint32_t stride() const = 0;
  virtual uint32_t count() const = 0;
  virtual uint32_t offset() const = 0;
};


};
#endif

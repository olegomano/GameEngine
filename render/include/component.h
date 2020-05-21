#ifndef _RENDER_COMPONENT_H_
#define _RENDER_COMPONENT_H_

#include <inttypes.h>
#include <ostream>
#include <unordered_map>
#include <vector>
#include "transform.h"
#include "scene_hook.h"

namespace render{
namespace scene{

enum Component : uint32_t{
  Drawable = 1,
  Camera = 1<<2,
  Transform = 1<<3
};
std::ostream& operator << (std::ostream& out, const Component& c);

struct IComponentArray{
  std::vector<uint32_t> entityIds;
  std::unordered_map<uint32_t,uint32_t> instanceMap;

  uint32_t size() const {
    return entityIds.size();
  }
  
  virtual void setTransformInstance(uint32_t instanceId, const ::Transform& t) = 0;
  virtual void setTransformEntity(uint32_t entityId, const ::Transform& t) = 0;
};

template<typename T>
struct ComponentArray : public IComponentArray{
  struct Component{
    ::Transform transform;
    T         component;
  };


  Component& operator[](uint32_t i){
    return components[i];
  }

  void setTransformInstance(uint32_t instance,const ::Transform& t) override {
    components[instance].transform = t;
  };

  void setTransformEntity(uint32_t entityId, const ::Transform& t ) override{
    setTransformInstance(instanceMap[entityId],t);
  }; 

  const auto& iterate() const{
    return components;
  }

  uint32_t createInstance(uint32_t entityId, const T& data = {}){
    uint32_t instanceId = components.size();
    instanceMap[entityId] = instanceId;
    components.push_back({ {} ,data});
    if(hook != nullptr){
      //hook->onCreated();
    }
    return instanceId;
  }

  std::vector<Component> components;
  SceneHook<T>* hook = nullptr; 
};

};
};


#endif

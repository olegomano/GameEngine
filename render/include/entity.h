#include "component.h"
#ifndef _H_RENDER_ENTITY_H_
#define _H_RENDER_ENTITY_H_

namespace render{
namespace scene{

class IAbstractScene;

uint32_t entityIdFromGlobalId(uint64_t id);
bool globalIdHasComponent(uint64_t globalId, Component c);
uint8_t componentIndex(Component c);


class Entity{
public:
  friend IAbstractScene;
  Entity();
  Entity(uint64_t globalId,IAbstractScene* owner);
  Entity(const Entity& other);
  Entity& operator=(const Entity& other);

  template<typename T>
  T& getComponent();
  
  template<typename T>
  void foreachComponent(T&& out){
    for(size_t i = 0; i < ComponentDescriptors_Length; i++){
      Component c = ComponentDescriptors[i].id;
      if(hasComponent(c)){
        out(c,m_owner->getComponentInstance(m_globalId,c));
      }
    }
  }

  bool hasComponent(Component c) const;
  uint32_t entityId() const;
  void addTag(const std::string& t){}

private:
  /* low bits are entity id high bits are mask for added components*/
  uint64_t m_globalId = -1;
  IAbstractScene* m_owner = nullptr;
};

}}
#endif

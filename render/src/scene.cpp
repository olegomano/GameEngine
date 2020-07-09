#include "../include/scene.h"
namespace render{
namespace scene{

uint32_t entityIdFromGlobalId(uint64_t id){
  return (uint32_t)(id&0xFFFFFFFF);
}

bool globalIdHasComponent(uint64_t globalId, Component c){
  uint64_t mask = globalId & (((uint64_t) c) << 32);
  return mask;
}

uint8_t componentIndex(Component c){
  uint32_t asInt = (uint32_t)c;
  for(int i = 1; i < 32; i++){
    uint32_t check = asInt >> i;
    if(check == 0) return (i - 1);
  }
  return -1;
}

std::ostream& operator << (std::ostream& out, const Component& c){
  switch(c){
  case Drawable:
   out << "Drawable";
   break;
  case Camera:
   out << "Camera";
   break;
  case Transform:
   out << "Transform";
   break;
  }
  return out;
}


auto& IAbstractScene::entityFilter(Component c){
  return m_componentMap[c];
}
  
bool IAbstractScene::componentInstanceExists(uint32_t entityId,Component c){
  return m_componentMap[c].count(entityId);
}

uint32_t IAbstractScene::componentInstanceId(uint32_t entityId, Component c){
  if(componentInstanceExists(entityId,c)){
    return -1;
  }
  return m_componentMap[c][entityId];
}

}
}

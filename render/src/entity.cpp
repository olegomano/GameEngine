#include "../include/scene.h"
#include "../include/camera.h"
#include "../include/drawable.h"

namespace render{
namespace scene{


Entity::Entity(){
  m_globalId = -1;
  m_owner = nullptr;
}

Entity:: Entity(uint64_t globalId,IAbstractScene* owner){
  m_globalId = globalId;
  m_owner = owner;
}

Entity::Entity(const Entity& other){
  m_globalId = other.m_globalId;
  m_owner = other.m_owner;
}

Entity& Entity::operator=(const Entity& other){
  m_globalId = other.m_globalId;
  m_owner = other.m_owner;
  return *this;
}

bool Entity::hasComponent(Component c) const{
  return globalIdHasComponent(m_globalId,c);
}

uint32_t Entity::entityId() const {
  return entityIdFromGlobalId(m_globalId);
}

template<>
ICamera& Entity::getComponent<ICamera>(){
  assert(hasComponent(Camera));
  return *((ICamera*)m_owner->getComponentInstance(m_globalId,Camera));
}

template<>
IDrawable& Entity::getComponent<IDrawable>(){
  assert(hasComponent(Drawable));
  return *((IDrawable*)m_owner->getComponentInstance(m_globalId,Drawable));
}

template<>
::Transform& Entity::getComponent<::Transform>(){
  assert(hasComponent(Transform));
  return *((::Transform*)m_owner->getComponentInstance(m_globalId,Transform));
}


}}

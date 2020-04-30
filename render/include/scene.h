#ifndef _RENDER_SCENE_H_
#define _RENDER_SCENE_H_
#include <unordered_map>
#include "transform.h"
#include "core.h"

namespace render{
namespace scene{

class IAbstractScene;

enum Component : uint32_t{
  Drawable = 1,
  Camera = 1<<1,
  Transform = 1<<2
};
std::ostream& operator << (std::ostream& out, const Component& c);

struct SiblingTable{
  uint32_t componentIndex[32];
};

struct IComponentArray{
  std::vector<uint64_t> globalIds;
  std::vector<::Transform> transforms;
    
  uint32_t size() const {
    return globalIds.size();
  }
};


template<typename T>
struct ComponentArray : public IComponentArray{
  std::vector<T> components;
  SceneHook<T>* hook = nullptr;

  T& operator[](uint32_t instanceIndex){
    return components[instanceIndex];
  }

  void push_back(uint64_t globalId,const T& t){
    this->globalIds.push_back(globalId);
    this->components.push_back(t);
    this->transforms.push_back(::Transform());
    this->siblingComponents.push_back({});
  } 
};


uint32_t entityIdFromGlobalId(uint64_t id);
bool globalIdHasComponent(uint64_t globalId, Component c);
uint8_t componentIndex(Component c);

class Entity{
public:
  friend IAbstractScene;
  Entity(uint64_t globalId,IAbstractScene* owner);
  Entity(const Entity& other);
  Entity& operator=(const Entity& other);

  template<typename T>
  T& getComponent();
  bool hasComponent(Component c);
  uint32_t entityId();
  void addTag(const std::string& t){}

private:
  Entity();

  uint64_t m_globalId;
  IAbstractScene* m_owner;
};

class IAbstractScene{
public:
  /**
   *allocates an instance of this component Type
   *returns the instanceId
   */
  virtual uint32_t createComponentInstance(uint32_t entityId,Component c) = 0;
  
  /*
   *returns a pointer to the instance of the component at 
   */
  virtual void* getComponentInstance(uint32_t entityId, Component c) = 0;
  
  /**
   *calculates the global position of all transforms
   */
  virtual void calculateTransformGlobals(){};

  /**
   *create an entity that contains componentList components
   */
  template<typename T>
  Entity createEntity(const T& componentList){
    uint32_t entityId = ++m_entityId;
    uint64_t guid = entityId;
    cprint_debug("Scene") << "Creating new Entity " << entityId << std::endl;
    for(const Component& c : componentList){
      uint32_t instanceId = createComponentInstance(entityId,c);
      if(instanceId == -1){
        cprint_error("Scene") << "Failed to create component " << c << std::endl;
        continue;
      }
      m_componentMap[c][entityId] = instanceId;
      guid |= ((uint64_t)(c) << 32);
    }
    m_allEntities.push_back(Entity(guid,this));
    return Entity(guid,this);
  }
  /**
   * returns all entityId that contain the following component
   */
  auto& entityFilter(Component c); 
  
  /**
   * returns true if the given entityId has a component of type c allocated
   */
  bool componentInstanceExists(uint32_t entityId,Component c);
  
  /**
   * returns the instance Id of the component for this entity
   */
  uint32_t componentInstanceId(uint32_t entityId, Component c);
    
protected:
  uint32_t m_entityId = 0;
  std::unordered_map<Component,std::unordered_map<uint32_t,uint32_t>> m_componentMap;
  std::unordered_map<std::string,std::vector<uint64_t>> m_entityTags;
  std::vector<Entity> m_allEntities;
};


template<typename T>
class SceneHook{
public:
  virtual void onCreated(T& t);
  virtual void onDeleted(T& t);
};

template<typename _T_Context>
class Scene : public IAbstractScene{
public:
  uint32_t createComponentInstance(uint32_t entityId, Component c) override;
  void* getComponentInstance(uint32_t entityId, Component c) override; 

  template<typename T,Component TYPE>
  void setHook(SceneHook<T>* hook){
    if constexpr (TYPE == Camera){
      m_cameras.hook = hook;
    }
    if constexpr (TYPE == Drawable){
      m_drawables.hook = hook;
    }
    static_assert(true,"");
  }

  template<Component C>
  auto& componentsOfType(){
    if constexpr (C == Transform){
      return m_transforms;
    }
    if constexpr (C == Camera){
      return m_cameras; 
    }
    if constexpr(C == Drawable){
      return m_drawables;
    }
    return {};
  }

private:
  
  ComponentArray<typename _T_Context::Camera>   m_cameras;
  ComponentArray<typename _T_Context::Drawable> m_drawables;
  SceneGraph m_sceneGraph;
  std::vector<IComponentArray*> m_components;
};

#include "scene.hpp"

}//namespace scene
};
#endif

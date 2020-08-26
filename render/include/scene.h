#ifndef _RENDER_SCENE_H_
#define _RENDER_SCENE_H_
#include <unordered_map>
#include "transform.h"
#include "core.h"
#include "scene_graph.h"
#include "component.h"
#include "drawable.h"
#include "camera.h"
#include "eventbus.h"
#include "entity.h"
#include "scene_events.h"

namespace render{
namespace scene{

class IAbstractScene{
public:
  /**
   *call to update global positions of all components
   */
  virtual void update() = 0;

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
      cprint_debug("Scene") << "Creating Component " << c << " instance id " << instanceId << std::endl; 
      if(instanceId == -1){
        cprint_error("Scene") << "Failed to create component " << c << std::endl;
        continue;
      }
      m_componentMap[c][entityId] = instanceId;
      guid |= ((uint64_t)(c) << 32);
    }
    m_allEntities.push_back(Entity(guid,this));
    onEntityCreated(Entity(guid,this));
    m_eventBus.send<Events::EntityCreated>( {Entity(guid,this)} ); 
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
  

  auto& eventBus(){
    return m_eventBus;
  }
protected:
  virtual void onEntityCreated(const Entity& e) = 0;
  
  core::eventbus::EventBus<Events> m_eventBus;
  uint32_t m_entityId = 0;
  std::unordered_map<Component,std::unordered_map<uint32_t,uint32_t>> m_componentMap; //component -> entity -> instance
  std::unordered_map<std::string,std::vector<uint64_t>> m_entityTags;
  std::vector<Entity> m_allEntities;
};

template<typename _T_Context>
class Scene : public IAbstractScene{
public:
  Scene(){
    m_componentArrayMap[Component::Drawable] = &m_drawables;
    m_componentArrayMap[Component::Camera] = &m_cameras;
  }

  void update(){
    m_sceneGraph.updateLinks();
  }

  uint32_t createComponentInstance(uint32_t entityId, Component c) override;
  void* getComponentInstance(uint32_t entityId, Component c) override; 

  template<Component C>
  const auto& componentsOfType(){
    if constexpr (C == Transform){
      return m_sceneGraph.globals();
    }
    if constexpr (C == Camera){
      return m_cameras.iterator(); 
    }
    if constexpr(C == Drawable){
      return m_drawables.iterator();
    }
    static_assert(true,"Unsupported Type");
  }

  const auto& drawables(){
    return m_drawables.iterator();
  }

  const auto& cameras(){
    return m_cameras.iterator();
  }
protected:
  void onEntityCreated(const Entity& e); 

private:
  
  ComponentArray<typename _T_Context::Camera>   m_cameras;
  ComponentArray<typename _T_Context::Drawable> m_drawables;
  SceneGraph m_sceneGraph;
  std::unordered_map<Component,IComponentArray*> m_componentArrayMap;
};

#include "scene.hpp"

}//namespace scene
};
#endif

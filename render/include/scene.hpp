
template<typename _T_Context>
uint32_t Scene<_T_Context>::createComponentInstance(uint32_t entityId,Component c){
  uint32_t instanceId = -1;
  switch(c){
    case Camera:
    instanceId = m_cameras.createInstance(entityId); 
    break;
    case Drawable:
    instanceId = m_drawables.createInstance(entityId);
    break;
    case Transform:
    instanceId = m_sceneGraph.createTransform();
    break;
  }    
  return instanceId;
}

 
template<typename _T_Context>
void* Scene<_T_Context>::getComponentInstance(uint32_t entityId, Component c){
  if(!componentInstanceExists(entityId,c)){
    return nullptr;    
  }
  uint32_t instanceId = m_componentMap[c][entityId];    
  switch(c){
  case Transform:
    return &m_sceneGraph.globals()[instanceId];
  case Camera:
    return &m_cameras[instanceId].component;
  case Drawable:
    return &m_drawables[instanceId].component;
  }
  return nullptr;
}


template<typename _T_Context>
void Scene<_T_Context>::onEntityCreated(const Entity& e){
  if( !e.hasComponent(Component::Transform) ) return;
  uint32_t transformIndex = m_componentMap[Transform][e.entityId()];

  for(size_t i = 0; i < ComponentDescriptors_Length; i++){
    Component c = ComponentDescriptors[i].id;
    if(e.hasComponent(c) && c != Component::Transform){
      m_sceneGraph.linkTransform(transformIndex,m_componentArrayMap[c],m_componentMap[c][e.entityId()]);
    }
  }
}

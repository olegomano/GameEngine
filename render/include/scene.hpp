
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

  for(int i = 0; i < ComponentTypeList.size(); i++){
    if(e.hasComponent(ComponentTypeList[i]) && ComponentTypeList[i] != Component::Transform){
      m_sceneGraph.linkTransform(transformIndex,m_componentArrayMap[ComponentTypeList[i]],m_componentMap[ComponentTypeList[i]][e.entityId()]);
    }
  }
}

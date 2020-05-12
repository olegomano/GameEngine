
template<typename _T_Context>
uint32_t Scene<_T_Context>::createComponentInstance(uint32_t entityId,Component c){
  switch(c){
    case Camera:
    uint32_t instanceId = m_cameras.createInstance(entityId); 
    return instanceId;
  }    
  return -1;
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


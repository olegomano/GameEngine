
template<typename _T_Context>
uint32_t Scene<_T_Context>::createComponentInstance(uint32_t entityId,Component c){{
  switch(c){
    case Camera:
      typename _T_Context::Camera c;
      uint32_t instanceId = m_cameras.size();
      m_cameras.push_back(entityId,c);
      if(m_cameras.hook != nullptr){
        m_cameras.hook->onCreated(m_cameras[instanceId]);
      }
      return instanceId;
    }    
    return -1;
  }
}
  
template<typename _T_Context>
void* Scene<_T_Context>::getComponentInstance(uint32_t entityId, Component c){
  if(!componentInstanceExists(entityId,c)){
    return nullptr;    
  }
  uint32_t instanceId = m_componentMap[c][entityId];    
  switch(c){
  case Transform:
    return &m_transforms[instanceId];
  case Camera:
    return &m_cameras[instanceId];
  case Drawable:
    return &m_drawables[instanceId];
  }
  return nullptr;
}


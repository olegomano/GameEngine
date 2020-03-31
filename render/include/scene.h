#ifndef _RENDER_SCENE_H_
#define _RENDER_SCENE_H_
#include <unordered_map>
#include "transform.h"


namespace render{

class IAbstractScene{
public:
  enum Object : uint32_t{
    Drawable = 1,
    Camera
  }
  virtual void* getCameraAbstract(uint64_t id) = 0;
  virtual void* getDrawableAbstract(uint64_t id) = 0;
  virtual void* getObjectAbstract(uint64_t id) = 0;
};

template<typename _T_Drawable>
class IScene : public IAbstractScene{
public:
 void* getCameraAbstract(uint64_t id) override{
    return &getCamera(id);
  }
  
  void* getDrawableAbstract(uint64_t id) override{
    return &getDrawable(id);
  }

  void* getObjectAbstract(uint64_t id) override{
    IAbstractScene::Object obj = objectFromId(id);
    uint32_t indx = indexFromId(id);
    switch(obj){
      case IAbstractScene::Object::Drawable:
        return getDrawableAbstract(indx);
      case IAbstractScene::Object::Camera:
        return getCameraAbstract(indx);
      default:
        return nullptr;
    }
  }
  
  SceneObject<ICamera> createCamera(){
    uint32_t indx = makeCamera();
    uint64_t id = newId(IAbstractScene::Object::Drawable);
    m_cameraMap[id] = indx;
    return SceneObject<ICamera>(*this,id);
  
  }

  
  SceneObject<_T_Drawable> createDrawable(const _T_Drawable& t){
    uint32_t indx = makeDrawable();
    _T_Drawable& d = getDrawableImpl(indx);
    d = t;
    uint64_t id = newId(IAbstractScene::Object::Drawable);
    m_drawableMap[id] = indx;
    return SceneObject<_T_Drawable>(*this,id);
  }

  ICamera& getCamera(uint64_t cameraId){
    uint32_t indx = m_cameraMap[cameraId];
    return getCameraImpl(indx);
  }

  _T_Drawable& getDrawable(uint64_t drawableId){
    uint32_t indx = m_drawableMap[drawableId];
    return getDrawableImpl(drawableId);
  }
protected:
  virtual uint32_t     makeCamera() = 0;
  virtual uint32_t     makeDrawable() = 0;
  virtual ICamera&     getCameraImpl(uint32_t indx) = 0;
  virtual _T_Drawable& getDrawableImpl(uint32_t indx) = 0;
private:
  uint64_t newId(IAbstractScene::Object obj){
    uint64_t id = m_id++;
    id |= (obj << 32);
    return id;
  }

  IAbstractScene::Object objectFromId(uint64_t id){
    IAbstractScene::Object obj = id >> 32;
    return obj;
  }

  uint32_t indexFromId(uint64_t id){
    return (uint32_t)(id&0xFFFFFFFF);
  }

private:
  std::unordered_map<uint32_t,uint32_t> m_cameraMap;
  std::unordered_map<uint32_t,uint32_t> m_drawableMap;
  uint32_t m_id = 0;
};


template<typename T>
class SceneObject{
public:
  SceneObject(IAbstractScene& sc,uint64_t indx) : m_scene(sc),m_index(indx){
  
  }

  T& object(){
    return *((T*)m_scene.getAbstractObject(m_index));
  }
  
  T* operator->(){
    &object();
  }
  
  Transform transform(){
    return m_transform;
  }

private:
  uint64_t m_index;
  IAbstractScene& m_scene;
  Transform m_transform;
};


template<typename _T_Context>
class Scene : public IScene<typename _T_Context::Drawable>{
public: 
  uint32_t     makeCamera(){
    typename _T_Context::Camera camera;
    m_cameras.push_back(camera);
    return m_cameras.size() -1;
  };

  uint32_t     makeDrawable(){
    typename _T_Context::Drawable d;
    m_drawables.push_back(d);
    return m_drawables.size() - 1;
   
  };

  ICamera&     getCameraImpl(uint32_t indx){
    return m_cameras[indx];
  };

  typename _T_Context::Drawable& getDrawableImpl(uint32_t indx){
    return m_drawables[indx];
  };
p
private:
  std::vector<typename _T_Context::Camera> m_cameras;
  std::vector<typename _T_Context::Drawable> m_drawables;
};


};
#endif

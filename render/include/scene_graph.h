#ifndef _RENDER_SCENE_GRAPH_
#define _RENDER_SCENE_GRAPH_
#include "transform.h"
#include "scene.h"
#include "component.h"
namespace render{
namespace scene{

struct PoseInfo{
  std::vector<uint32_t> instanceId;
  std::vector<IComponentArray*> componentArray;
  uint32_t parent = -1;
};

class SceneGraph{
public:
  void calculateGlobals(){}
  auto& globals() {return m_globalPositions;}
  
  uint32_t createTransform(){
    ::Transform t;
    return createTransform(t);
  }

  uint32_t createTransform(::Transform& t, uint32_t parent = -1){
    uint32_t id = m_localPositions.size();
    m_localPositions.push_back(t);
    m_globalPositions.push_back(t);
    
    PoseInfo pi;
    pi.parent = parent;
    m_poseInfo.push_back(pi);

    return id;
  }

  void updateLinks(){
    for(int p =0; p < m_localPositions.size(); p++){
      const PoseInfo& pinfo = m_poseInfo[p];
      const ::Transform& transform = m_globalPositions[p];

      for(int i = 0; i < pinfo.instanceId.size(); i++){
        pinfo.componentArray[i]->setTransformInstance(pinfo.instanceId[i],transform);
      }
    } 
  }

  void linkTransform(uint32_t transformIndex, IComponentArray* array, uint32_t instance){
    m_poseInfo[transformIndex].instanceId.push_back(instance);
    m_poseInfo[transformIndex].componentArray.push_back(array);
  } 
private:
  //alligned arrays, structure of arrays type stuff
  std::vector<::Transform> m_localPositions;
  std::vector<::Transform> m_globalPositions;
  std::vector<PoseInfo>    m_poseInfo; //0 index is root
};

}
};



#endif

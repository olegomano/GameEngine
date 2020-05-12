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
  
  uint32_t createTransform(::Transform& t, uint32_t parent = -1){
    uint32_t id = m_localPositions.size();
    m_localPositions.push_back({});
    m_globalPositions.push_back({});
    
    PoseInfo pi;
    pi.parent = parent;
    m_poseInfo.push_back(pi);

    return id;
  }

  void updateLinks(){
  
  }

  void linkTransform(uint32_t transformIndex, IComponentArray* array, uint32_t instance){
    m_poseInfo[transformIndex].instanceId.push_back(instance);
    m_poseInfo[transformIndex].componentArray.push_back(array);
  } 
private:
  std::vector<::Transform> m_localPositions;
  std::vector<::Transform> m_globalPositions;
  std::vector<PoseInfo>    m_poseInfo; //0 index is root
};

}
};



#endif

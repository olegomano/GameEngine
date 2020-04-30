#ifndef _RENDER_SCENE_GRAPH_
#define _RENDER_SCENE_GRAPH_
#include "scene.h"

namespace render{
namespace scene{

struct LinkedComponent{
  IComponentArray* array;
  uint32_t         instanceId;
}

struct PoseInfo{
  std::vector<uint32_t>        children;
  std::vector<LinkedComponent> links;
  uint32_t parent = -1;
}

class SceneGraph{
public:
  void calculateGlobals();
  const auto& globals() const {return m_globalPositions;}
  uint32_t createTransform(::Transform& t, uint32_t parent = -1);
  void linkTransform(uint32_t transformIndex, const LinkedComponent& link); 
private:
  std::vector<::Transform> m_localPositions;
  std::vector<::Transform> m_globalPositions;
  std::vector<PoseInfo>    m_poseInfo; //0 index is root
};

}
};



#endif

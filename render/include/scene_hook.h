#ifndef RENDER_SCENEHOOK_H_
#define RENDER_SCENEHOOK_H_

#include <cinttypes>

namespace render{
namespace scene{

template<typename T>
class SceneHook{
public:
  virtual void onCreated(uint32_t entityId, uint32_t componentid, T& inst) = 0;
  virtual void onDeleted(uint32_t entityId, uint32_t componenid, T& inst) = 0;
};



}
}




#endif

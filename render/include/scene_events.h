#ifndef _RENDER_SCENE_EVENTS_H_
#define _RENDER_SCENE_EVENTS_H_
#include "camera.h"
#include "transform.h"
#include "scene.h"
#include "entity.h"

namespace render{
namespace scene{

struct Events{
  enum Type{
    ENTITY_CREATED = 0,
    ENTITY_DESTROYED
  };
  
  struct EntityCreated{
    Entity e;
  };

  struct EntityDestroyed{
    Entity e;
  };

  
  template<typename T>
  struct type_id;

};


template<>
struct Events::type_id<Events::EntityCreated>{static constexpr uint64_t value = Events::ENTITY_CREATED;};
  
template<>
struct Events::type_id<Events::EntityDestroyed>{static constexpr uint64_t value = Events::ENTITY_DESTROYED;};

}
}


#endif

#include "../include/scene_events.h"

template<>
static constexpr uint64_t render::scene::Events::type_id<render::scene::Events::EntityCreated>(){return render::scene::ENTITY_CREATED;}

template<>
static constexpr uint64_t render::scene::Events::type_id<render::scene::Events::EntityDestroyed>(){return ENTITY_DESTROYED;}


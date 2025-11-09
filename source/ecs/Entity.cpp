#include "ecs/Entity.hpp"
#include "ecs/EntityManager.hpp"

namespace ecs {
    bool Entity::valid() const {
        return mgr_ && id_ < MAX_ENTITIES && mgr_->isAlive(id_);
    }
}

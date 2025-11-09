#ifndef ECS_SYSTEM_HPP
#define ECS_SYSTEM_HPP

#include <set>
#include "ecs/Entity.hpp"  

namespace ecs {
    class System {
        public:
            std::set<Entity> mEntities;  
    };
}
#endif
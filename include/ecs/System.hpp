#ifndef ECS_SYSTEM_HPP
#define ECS_SYSTEM_HPP

#include <set>
#include "ecs/Types.hpp"

namespace ecs {

class System {
public:
    std::set<Entity> mEntities;
};

} 

#endif 
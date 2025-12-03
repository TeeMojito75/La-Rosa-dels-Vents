#ifndef ECS_SYSTEM_HPP
#define ECS_SYSTEM_HPP

#include <vector>
#include "ecs/Entity.hpp"  

namespace ecs {
    class System
    {
    public:
        std::vector<Entity> mEntities;

        void AddEntity(const Entity& e)
        {
            for (const auto& ent : mEntities)
            {
                if (ent == e) return;   
            }

            mEntities.push_back(e);
        }
        
        void RemoveEntity(Entity e)
        {
            for (size_t i = 0; i < mEntities.size(); ++i)
            {
                if (mEntities[i] == e)
                {
                    mEntities[i] = mEntities.back();
                    mEntities.pop_back();
                    return;
                }
            }
        }
    };
}
#endif
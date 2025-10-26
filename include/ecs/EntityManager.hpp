#ifndef ECS_ENTITYMANAGER_HPP
#define ECS_ENTITYMANAGER_HPP

#include <queue>
#include <array>
#include "ecs/Types.hpp"

namespace ecs 
{

    class EntityManager 
    {
        
        public:
            EntityManager();
            // Contructora i destructora
            Entity createEntity();
            void destroyEntity(Entity e);

            // Signature (quin components té una entitat)
            void setSignature(Entity e, Signature s);
            Signature getSignature(Entity e) const;

            std::uint32_t getEntityLivingCount() const;

        private:
            // Coa de les Ids d'entitats no utilitzades
            std::queue<Entity> mAvailableEntitites;

            // Array de les firmes on l'índex correspon a l'id d'una entitat
            std::array<Signature, MAX_ENTITIES> mSignatures;

            // Total entitats vives - nombre utilitzades per saber restants
            std::uint32_t mLivingEntityCount = 0;  
    };    
}

#endif
#ifndef ECS_ENTITYMANAGER_HPP
#define ECS_ENTITYMANAGER_HPP
#include <queue>
#include <array>
#include "ecs/Utils.hpp"
#include "ecs/Entity.hpp"

namespace ecs 
{

    class EntityManager 
    {
        
        public:
            EntityManager();
            // Contructora i destructora
            Entity createEntity();
            void destroyEntity(Entity entity);

            // Signature (quin components té una entitat)
            void setSignature(Entity entity, Signature signature);
            Signature getSignature(Entity entity) const;

            std::uint32_t getEntityLivingCount() const;

            bool isAlive(EntityId id) const;

        private:
            // Coa de les Ids d'entitats no utilitzades
            std::queue<ecs::EntityId> mAvailableEntitites;

            // Estat de vida i firma per cada entitat
            std::array<bool, MAX_ENTITIES> mAlive{};
            std::array<Signature, MAX_ENTITIES> mSignatures{};

            // Total entitats vives - nombre utilitzades per saber restants
            std::uint32_t mLivingEntityCount = 0;  
    };    
}
#endif
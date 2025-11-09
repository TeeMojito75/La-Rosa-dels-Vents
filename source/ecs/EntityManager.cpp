#include "ecs/EntityManager.hpp"
#include <cassert>

namespace ecs {

    EntityManager::EntityManager()
    {
        // Inicialitzem la pool de IDs i l'estat de firmes
        for (EntityId id = 0; id < MAX_ENTITIES; ++id) 
        {
            mAvailableEntitites.push(id);
            mAlive[id] = false;
            mSignatures[id].reset();
        }
        mLivingEntityCount = 0;
    }

    Entity EntityManager::createEntity()
    {
        assert(mLivingEntityCount < MAX_ENTITIES && "Too many entities in existence.");
        assert(!mAvailableEntitites.empty() && "No available entity IDs.");

        const EntityId id = mAvailableEntitites.front();
        mAvailableEntitites.pop();

        mAlive[id] = true;
        mSignatures[id].reset();
        ++mLivingEntityCount;

        return Entity(id, this);
    }

    void EntityManager::destroyEntity(Entity entity)
    {
        const EntityId id = entity.id();
        assert(id < MAX_ENTITIES && "Entity id out of range.");
        assert(mAlive[id] && "Destroying a non-living entity.");

        // Neteja la firma, la marca com a morta i retorna l'id a la pool
        mSignatures[id].reset();
        mAlive[id] = false;

        mAvailableEntitites.push(id);
        assert(mLivingEntityCount > 0 && "Living entity count underflow.");
        --mLivingEntityCount;
    }

    void EntityManager::setSignature(Entity entity, Signature signature)
    {
        const EntityId id = entity.id();
        assert(id < MAX_ENTITIES && "Entity id out of range.");
        assert(mAlive[id] && "Setting signature on non-living entity.");

        mSignatures[id] = signature;
    }

    Signature EntityManager::getSignature(Entity entity) const
    {
        const EntityId id = entity.id();
        assert(id < MAX_ENTITIES && "Entity id out of range.");
        return mSignatures[id];
    }

    std::uint32_t EntityManager::getEntityLivingCount() const
    {
        return mLivingEntityCount;
    }

    bool EntityManager::isAlive(EntityId id) const
    {
        assert(id < MAX_ENTITIES && "Entity id out of range.");
        return mAlive[id];
    }

}

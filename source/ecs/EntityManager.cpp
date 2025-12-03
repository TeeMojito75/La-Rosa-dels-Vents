#include <cassert>
#include "ecs/EntityManager.hpp"

namespace ecs {

    EntityManager::EntityManager(ComponentManager& compMgr, SystemManager& sysMgr)
        : mComponentMgr(compMgr), mSystemMgr(sysMgr)
    {
        for (EntityId id = 0; id < MAX_ENTITIES; ++id)
        {
            mAvailableEntities.push(id);
            mAlive[id] = false;
            mSignatures[id].reset();
        }
    }

    Entity EntityManager::CreateEntity()
    {
        assert(!mAvailableEntities.empty() && "Max entity count reached!");

        EntityId id = mAvailableEntities.front();
        mAvailableEntities.pop();

        mAlive[id] = true;
        mSignatures[id].reset();

        return Entity(id, this);
    }

    void EntityManager::DestroyEntity(Entity entity)
    {
        EntityId id = entity.id();
        assert(id < MAX_ENTITIES);
        assert(mAlive[id] && "Destroying non-living entity!");

        // Notificar sistemes i components
        mComponentMgr.EntityDestroyed(entity);
        mSystemMgr.EntityDestroyed(entity);

        mAlive[id] = false;
        mSignatures[id].reset();

        mAvailableEntities.push(id);
    }

    void EntityManager::SetSignature(Entity entity, const Signature& signature)
    {
        EntityId id = entity.id();
        assert(id < MAX_ENTITIES);

        mSignatures[id] = signature;
        mSystemMgr.EntitySignatureChanged(entity, signature);
    }

} // namespace ecs


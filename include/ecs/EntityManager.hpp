#ifndef ECS_ENTITYMANAGER_HPP
#define ECS_ENTITYMANAGER_HPP

#include <queue>
#include <array>
#include <cassert>

#include "ecs/Utils.hpp"
#include "ecs/Entity.hpp"
#include "ecs/ComponentManager.hpp"
#include "ecs/SystemManager.hpp"

namespace ecs 
{
    class EntityManager
    {
        public:
            EntityManager(ComponentManager& compMgr, SystemManager& sysMgr)
                : mComponentMgr(compMgr), mSystemMgr(sysMgr)
            {
                for (EntityId id = 0; id < MAX_ENTITIES; ++id)
                {
                    mAvailableEntities.push(id);
                    mAlive[id] = false;
                    mSignatures[id].reset();
                }
            }


            // Creació de les entitats
            Entity CreateEntity()
            {
                assert(!mAvailableEntities.empty() && "Too many entities created!");

                EntityId id = mAvailableEntities.front();
                mAvailableEntities.pop();

                mAlive[id] = true;
                mSignatures[id].reset();

                return Entity(id, this);
            }


            // Destrucció entitats
            void DestroyEntity(Entity entity)
            {
                EntityId id = entity.id();
                assert(id < MAX_ENTITIES);
                assert(mAlive[id] && "Destroying non-living entity!");

                // Notifica a sistemes i components
                mComponentMgr.EntityDestroyed(entity);
                mSystemMgr.EntityDestroyed(entity);

                mAlive[id] = false;

                // Alliberar ID
                mAvailableEntities.push(id);

                // Netejar firma
                mSignatures[id].reset();
            }


            // Gestio Firmes entitat
            void SetSignature(Entity entity, const Signature& signature)
            {
                EntityId id = entity.id();
                assert(id < MAX_ENTITIES);

                mSignatures[id] = signature;
                mSystemMgr.EntitySignatureChanged(entity, signature);
            }

            const Signature& GetSignature(Entity entity) const
            {
                return mSignatures[entity.id()];
            }

        private:
            std::queue<EntityId> mAvailableEntities;
            std::array<bool, MAX_ENTITIES> mAlive{};
            std::array<Signature, MAX_ENTITIES> mSignatures{};

            ComponentManager& mComponentMgr;
            SystemManager& mSystemMgr;
    };

} // namespace ecs

#endif

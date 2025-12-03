#ifndef ECS_COMPONENTARRAY_HPP
#define ECS_COMPONENTARRAY_HPP

#include <array>
#include "ecs/Utils.hpp"
#include "ecs/Entity.hpp"

namespace ecs 
{

    class IComponentArray
    {
        public:
            virtual ~IComponentArray() = default;
            virtual void EntityDestroyed(Entity entity) = 0;
    };

    template<typename T>
    class ComponentArray : public IComponentArray 
    {
        public:
            // Insereix o reemplaça el component per a una entitat
            void InsertData(Entity entity, const T& component)
            {
                const EntityId id = entity.id();
                assert(id < MAX_ENTITIES && "EntityId out of range in ComponentArray::InsertData");

                mComponentArray[id] = component;
                mHasComponent[id] = true;
            }

            // Elimina el component (si existeix) per a una entitat
            void RemoveData(Entity entity)
            {
                const EntityId id = entity.id();
                assert(id < MAX_ENTITIES && "EntityId out of range in ComponentArray::RemoveData");

                mHasComponent[id] = false;
                // No cal netejar mComponentArray[id], el valor es considera "no vàlid" si mHasComponent[id] == false
            }

            // Accedeix al component d'una entitat (ha d'existir)
            T& GetData(Entity entity)
            {
                const EntityId id = entity.id();
                assert(id < MAX_ENTITIES && "EntityId out of range in ComponentArray::GetData");
                assert(mHasComponent[id] && "Component does not exist for entity in ComponentArray::GetData");

                return mComponentArray[id];
            }

            const T& GetData(Entity entity) const
            {
                const EntityId id = entity.id();
                assert(id < MAX_ENTITIES && "EntityId out of range in ComponentArray::GetData (const)");
                assert(mHasComponent[id] && "Component does not exist for entity in ComponentArray::GetData (const)");

                return mComponentArray[id];
            }

            // Quan una entitat es destrueix, eliminam el component si el tenia
            void EntityDestroyed(Entity entity) override
            {
                const EntityId id = entity.id();
                if (id < MAX_ENTITIES) {
                    mHasComponent[id] = false;
                }
            }

            // (Opcional) saber si una entitat té aquest component
            bool HasComponent(Entity entity) const
            {
                const EntityId id = entity.id();
                return (id < MAX_ENTITIES) && mHasComponent[id];
            }

        private:
            std::array<T, MAX_ENTITIES> mComponentArray{};
            std::array<bool, MAX_ENTITIES> mHasComponent{}; 
    };

} // namespace ecs

#endif

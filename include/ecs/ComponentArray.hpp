#ifndef ECS_COMPONENTARRAY_HPP
#define ECS_COMPONENTARRAY_HPP
#include <array>
#include <unordered_map>
#include "ecs/Utils.hpp"
#include "ecs/Entity.hpp"

namespace ecs {
    class IComponentArray {
        public:
            virtual ~IComponentArray() = default;
            virtual void EntityDestroyed(Entity entity) = 0;
    };

    template<typename T>
    class ComponentArray : public IComponentArray {
        public:
            void InsertData(Entity e, T value)   { InsertData(e.id(), std::move(value)); }
            void RemoveData(Entity e)            { RemoveData(e.id()); }
            T&   GetData(Entity e)               { return GetData(e.id()); }
            bool HasData(Entity e) const         { return HasData(e.id()); }

            
            void InsertData(EntityId entity, T component) {
                assert(mEntityToIndexMap.find(entity) == mEntityToIndexMap.end() && "Component added to same entity more than once.");
                
                size_t newIndex = mSize;
                
                mEntityToIndexMap[entity] = newIndex;
                mIndexToEntityMap[newIndex] = entity;
                mComponentArray[newIndex] = std::move(component);
                
                ++mSize;
            }

            void RemoveData(EntityId id) {
                assert(mEntityToIndexMap.find(id) != mEntityToIndexMap.end() && "Removing non-existent component.");
            
                size_t indexOfRemovedEntity = mEntityToIndexMap[id];
                size_t indexOfLastElement   = mSize - 1;
                mComponentArray[indexOfRemovedEntity] = std::move(mComponentArray[indexOfLastElement]);

                EntityId entityOfLastElement = mIndexToEntityMap[indexOfLastElement];
                mEntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
                mIndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

                mEntityToIndexMap.erase(id);
                mIndexToEntityMap.erase(indexOfLastElement);
                --mSize;
            }

            T& GetData(EntityId id) {
                auto it = mEntityToIndexMap.find(id);
               
                assert(it != mEntityToIndexMap.end() && "Retrieving non-existent component.");
                
                return mComponentArray[it->second];
            }

            bool HasData(EntityId entity) const {
                return mEntityToIndexMap.find(entity) != mEntityToIndexMap.end();
            }

            void EntityDestroyed(Entity entity) override {
                auto it = mEntityToIndexMap.find(entity.id());
                if (it != mEntityToIndexMap.end()) RemoveData(entity.id());
            }

        private:
                std::array<T, MAX_ENTITIES> mComponentArray{};
                // Diccionari d'una id d'entitat a un índex de l'array
                std::unordered_map<EntityId, size_t> mEntityToIndexMap;
                // Diccionari invers al superior
                std::unordered_map<size_t, EntityId> mIndexToEntityMap;
                // Tamany total d'entrades vàlides del array
                size_t mSize = 0;
    };
}
#endif
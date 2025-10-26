#ifndef ECS_COMPONENTARRAY_HPP
#define ECS_COMPONENTARRAY_HPP

#include <array>
#include <unordered_map>
#include <cassert>
#include "ecs/Types.hpp"

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
            void InsertData(Entity entity, T component)
            {
                assert(mEntityToIndexMap.find(entity) == mEntityToIndexMap.end() && "Component added to same entity more than once.");

                // Posem un nou entry al final i actualitzem el map
                size_t newIndex = mSize;
                mEntityToIndexMap[entity] = newIndex;
                mIndexToEntityMap[newIndex] = entity;
                mComponentArray[newIndex] = component;
                ++mSize;
            }
            
            void RemoveData(Entity entity)
            {
                assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Removing non-existent component.");

                // Copiem l'element al final a la posició de l'element eliminat per mantenir densitat
                size_t indexOfRemovedEntity = mEntityToIndexMap[entity];
                size_t indexOfLastElement = mSize - 1;
                mComponentArray[indexOfRemovedEntity] = mComponentArray[indexOfLastElement];

                // Actualitzem el diccionari per que apunti a l'elemnt mogut
                Entity entityOfLastElement = mIndexToEntityMap[indexOfLastElement];
                mEntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
                mIndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

                mEntityToIndexMap.erase(entity);
                mIndexToEntityMap.erase(entity);

                --mSize;
            }

            T& GetData(Entity entity) 
            {
                assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Retrieving non-existent component.");

                // Retornem una referència al component de l'entitat
                return mComponentArray[mEntityToIndexMap[entity]];
            }

            void EntityDestroyed(Entity entity) override
            {
                if (mEntityToIndexMap.find(entity) != mEntityToIndexMap.end())
                {
                    // Eliminem el component de l'entitat si existia
                    removeData(entity);
                }
            }

        private:
        
        std::array<T, MAX_ENTITIES> mComponentArray;

        // Diccionari d'una id d'entitat a un índex de l'array
        std::unordered_map<Entity, size_t> mEntityToIndexMap;

        // Diccionari invers al superior
        std::unordered_map<size_t, Entity> mIndexToEntityMap;

        // Tamany total d'entrades vàlides del array
        size_t mSize;

    };
}


#endif
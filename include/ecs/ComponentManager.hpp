#ifndef ECS_COMPONENTMANAGER_HPP
#define ECS_COMPONENTMANAGER_HPP

#include <array>
#include <unordered_map>
#include <cassert>
#include "ecs/Types.hpp"
#include <memory>
#include "ComponentArray.hpp"

namespace ecs 
{
    class ComponentManager
    {
        public:
            template<typename T>
            void RegisterComponent()
            {
                const char* typeName = typeid(T).name();

                assert(mComponentTypes.find(typeName) == mComponentTypes.end() && "Registering component type more than once.");

                // Afegim el tipus de component al diccionari de tipus
                mComponentTypes.insert({typeName, mNextComponentType});

                // Creem un punter d'array de component i l'afegim al diccionari
                mComponentArrays.insert({typeName,std::make_shared<ComponentArray<T>>()});

                ++mNextComponentType;
            }

            template<typename T>
            ComponentType GetComponentType()
            {
                assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use.");

                // Return this component's type - used for creating signatures
                return mComponentTypes[typeName];

            }

            template<typename T>
            void AddComponent(Entity entity, T component)
            {
                // Afegim un component a l'array d'una entitat
                GetComponentArray<T>()->InsertData(entity, component);
            }

            template<typename T>
            void RemoveComponent(Entity entity)
            {
                // Eliminem un component de l'array d'una entitat
                GetComponentArray<T>()->RemoveData(entity);
            }

            template<typename T>
            T& GetComponent(Entity entity)
            {
                // Obtenim la referència d'un component d'una entitat
                return GetComponentArray<T>()->GetData(entity);
            }

            void EntityDestroyed(Entity entity)
            {
                // Avisem a cada array de components que una entitat s'ha eliminat
                // Si té un component per aquella entitat, l'elimina
                for (auto const& pair : mComponentArrays)
                {
                    auto const& component = pair.second;

                    component->EntityDestroyed(entity);
                }
            }


        private:
            // Diccionari d'un pointer d'string a un componentype, util per gestionar els diferents tipus de components disponibles
            std::unordered_map<const char*, ComponentType> mComponentTypes{};

            // Diccionari d'un type string pointer a un array de components
            std::unordered_map<const char*, std::shared_ptr<IComponentArray>> mComponentArrays{};

            // El tipus de component per assignar al següent - comença al 0
            ComponentType mNextComponentType{};

            // Funció útil per obtenir el punter a l'array de components de tipus T
            template<typename T>
            std::shared_ptr<ComponentArray<T>> GetComponentArray()
            {
                const char* typeName = typeid(T).name();

                assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use.");

                return std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[typeName]);
            }

    };
}

#endif
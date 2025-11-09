#ifndef ECS_COMPONENTMANAGER_HPP
#define ECS_COMPONENTMANAGER_HPP
#include <unordered_map>
#include <memory>
#include <cassert>
#include "ecs/Utils.hpp"
#include "ecs/Entity.hpp"
#include "ecs/ComponentArray.hpp"

namespace ecs 
{
    using ComponentType = std::uint8_t;

    class ComponentManager
    {
        public:
            template<typename T>
            void RegisterComponent()
            {
                auto tid = ecs::get_type_id<T>();

                assert(mComponentTypes.find(tid) == mComponentTypes.end() && "Registering component type more than once.");
                
                // Afegim el tipus de component al diccionari de tipus
                mComponentTypes[tid] = mNextComponentType++;

                // Creem un punter d'array de component i l'afegim al diccionari
                mComponentArrays.insert({ tid, std::make_shared<ComponentArray<T>>() });
            }

            template<typename T>
            ComponentType GetComponentType()
            {
                auto tid = ecs::get_type_id<T>();

                assert(mComponentTypes.find(tid) != mComponentTypes.end() && "Component not registered before use.");
                // Return this component's type - used for creating signatures
                return mComponentTypes[tid];

            }

            template<typename T>
            void AddComponent(Entity entity, T component)
            {
                // Afegim un component a l'array d'una entitat
                GetComponentArray<T>()->InsertData(entity, std::move(component));
            }

            template<typename T>
            void RemoveComponent(Entity entity)
            {
                // Eliminem un component de l'array d'una entitat
                GetComponentArray<T>()->RemoveData(entity);
            }
            
            template<typename T>
            T& GetComponent(Entity e) {
                // Retornem un component de l'array d'una entitat
                return GetComponentArray<T>()->GetData(e);
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

            // Funció útil per obtenir el punter a l'array de components de tipus T
            template<typename T>
            std::shared_ptr<ComponentArray<T>> GetComponentArray() 
            {     
                auto tid = ecs::get_type_id<T>();
                assert(mComponentTypes.find(tid) != mComponentTypes.end() && "Component not registered before use.");
                return std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[tid]);
            }


        private:
            // Diccionari d'un pointer d'string a un componentype, util per gestionar els diferents tipus de components disponibles
            std::unordered_map<ecs::TypeId, ecs::ComponentType> mComponentTypes{};

            // Diccionari d'un type string pointer a un array de components
            std::unordered_map<ecs::TypeId, std::shared_ptr<IComponentArray>> mComponentArrays{};

            // El tipus de component per assignar al següent - comença al 0
            ComponentType mNextComponentType = 0;

    };
}
#endif
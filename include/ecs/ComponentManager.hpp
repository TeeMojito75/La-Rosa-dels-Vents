#ifndef ECS_COMPONENTMANAGER_HPP
#define ECS_COMPONENTMANAGER_HPP

#include <array>
#include <memory>
#include <cassert>

#include "ecs/Utils.hpp"
#include "ecs/ComponentArray.hpp"
#include "ecs/Entity.hpp"

namespace ecs {

    class ComponentManager
    {
        public:
            // Registre de components
            template<typename T>
            void RegisterComponent()
            {
                TypeId typeId = get_type_id<T>();
                assert(typeId < MAX_COMPONENTS && "Too many component types registered!");
                assert(mComponentArrays[typeId] == nullptr && "Component type already registered!");

                // Creem un ComponentArray<T> i l'emmagatzemem
                mComponentArrays[typeId] = std::make_unique<ComponentArray<T>>();
            }

            // Gestió de components de l'entitat
            template<typename T>
            void AddComponent(Entity entity, const T& component)
            {
                GetArray<T>()->InsertData(entity, component);
            }

            template<typename T>
            void RemoveComponent(Entity entity)
            {
                GetArray<T>()->RemoveData(entity);
            }

            template<typename T>
            T& GetComponent(Entity entity)
            {
                return GetArray<T>()->GetData(entity);
            }

            template<typename T>
            const T& GetComponent(Entity entity) const
            {
                return GetArray<T>()->GetData(entity);
            }

            // Eliminació de components a l'hora de destruir una entitat
            void EntityDestroyed(Entity entity)
            {
                for (auto& arrayPtr : mComponentArrays)
                {
                    if (arrayPtr)
                    {
                        arrayPtr->EntityDestroyed(entity);
                    }
                }
            }

        private:

                // Array d'arrays de components indexats per TypeId
                std::array<std::unique_ptr<IComponentArray>, MAX_COMPONENTS> mComponentArrays{};

                // Cast a componentArray
                template<typename T>
                ComponentArray<T>* GetArray()
                {
                    TypeId typeId = get_type_id<T>();
                    assert(typeId < MAX_COMPONENTS);
                    assert(mComponentArrays[typeId] != nullptr && "Component not registered before use!");

                    return static_cast<ComponentArray<T>*>(mComponentArrays[typeId].get());
                }
    };

} 

#endif

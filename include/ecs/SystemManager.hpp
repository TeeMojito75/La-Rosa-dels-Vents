#ifndef ECS_SYSTEMMANAGER_HPP
#define ECS_SYSTEMMANAGER_HPP
#include <memory>
#include <unordered_map>
#include <utility>
#include "ecs/Utils.hpp"
#include "ecs/System.hpp"
#include "ecs/Entity.hpp"

namespace ecs 
{
    class SystemManager
    {
        public:

            template<typename T, typename... Args>
            T* RegisterSystem(Args&&... args)
            {
                TypeId typeId = get_type_id<T>();
                assert(typeId < MAX_SYSTEMS && "Too many systems!");

                assert(mSystems[typeId] == nullptr && "System already registered!");

                mSystems[typeId] = std::make_unique<T>(std::forward<Args>(args)...);
                return static_cast<T*>(mSystems[typeId].get());
            }

            template<typename T>
            void SetSignature(const Signature& signature)
            {
                TypeId typeId = get_type_id<T>();
                assert(typeId < MAX_SYSTEMS);

                mSignatures[typeId] = signature;
            }

            void EntityDestroyed(Entity entity)
            {
                for (auto& sys : mSystems)
                {
                    if (sys)
                    {
                        sys->RemoveEntity(entity);
                    }
                }
            }

            void EntitySignatureChanged(Entity entity, const Signature& entitySignature)
            {
                for (TypeId i = 0; i < MAX_SYSTEMS; ++i)
                {
                    auto& sys = mSystems[i];
                    if (!sys)
                    {
                        continue;
                    }

                    bool matches = (entitySignature & mSignatures[i]) == mSignatures[i];

                    if (matches)
                    {
                        sys->AddEntity(entity);
                    }
                    else
                    {
                        sys->RemoveEntity(entity);
                    }
                }
            }

        private:
            std::array<std::unique_ptr<System>, MAX_SYSTEMS> mSystems{};
            std::array<Signature, MAX_SYSTEMS> mSignatures{};
    };

}
#endif
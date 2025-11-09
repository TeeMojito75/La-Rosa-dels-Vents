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
            std::shared_ptr<T> RegisterSystem(Args&&... args)
            {
                auto tid = ecs::get_type_id<T>();
                assert(mSystems.find(tid) == mSystems.end() && "Registering system more than once.");

                auto system = std::make_shared<T>(std::forward<Args>(args)...);
                mSystems.insert({ tid, std::static_pointer_cast<System>(system) });
                return system;
            }

            template<typename T>
            void SetSignature(Signature signature) 
            {
                auto tid = ecs::get_type_id<T>();

                assert(mSystems.find(tid) != mSystems.end() && "System used before registered.");
                // Setejar la firma per aquest sistema
                mSignatures[tid] = signature;
            }

            void EntityDestroyed(Entity entity);
            void EntitySignatureChanged(Entity entity, Signature entitySignature);

        private:
            // Diccionari de punter del tipus de sistema a una firma
            std::unordered_map<ecs::TypeId, Signature> mSignatures; 

            // Diccionari de punter del tipus de sistema a un punter de sistema
            std::unordered_map<ecs::TypeId, std::shared_ptr<System>> mSystems{};   
    };

}
#endif
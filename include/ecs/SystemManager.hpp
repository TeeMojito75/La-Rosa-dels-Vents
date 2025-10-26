#ifndef ECS_SYSTEMMANAGER_HPP
#define ECS_SYSTEMMANAGER_HPP

#include <memory>
#include <unordered_map>
#include <typeindex>
#include <cassert>
#include "ecs/Types.hpp"
#include "ecs/System.hpp"

namespace ecs 
{
    class SystemManager 
    {
        public:
            template<typename T>
            std::shared_ptr<T> RegisterSystem() 
            {
                const char* typeName = typeid(T).name();

                assert(mSystems.find(typeName) == mSystems.end() && "Registering system more than once.");

                // Cream un punter al sistema i el retornem per utilitzarlo externament
                auto system = std::make_shared<T>();
                mSystems.insert({typeName, system});
                return system;
            }

            template<typename T>
            void SetSignature(Signature signature) 
            {
                const char* typeName = typeid(T).name();

                assert(mSystems.find(typeName) != mSystems.end() && "System used before registered.");

                // Setejar la firma per aquest sistema
                mSignatures.insert({typeName, signature});
            }

            void EntityDestroyed(Entity entity);
            void EntitySignatureChanged(Entity entity, Signature entitySignature);

        private:
            // Diccionari de punter del tipus de sistema a una firma
            std::unordered_map<std::type_index, Signature> mSignatures; 

            // Diccionari de punter del tipus de sistema a un punter de sistema
            std::unordered_map<std::type_index, std::shared_ptr<System>> mSystems{};   
    };

} 

#endif 

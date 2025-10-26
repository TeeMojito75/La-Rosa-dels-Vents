#include "ecs/SystemManager.hpp"

namespace ecs 
{

    void SystemManager::EntityDestroyed(Entity entity) 
    {
        // Esborra una entitat destruida de totes les llistes de sistema
        for (auto &pair : mSystems) 
        {
            auto const& system = pair.second;
            system->mEntities.erase(entity);
        }
    }

    void SystemManager::EntitySignatureChanged(Entity entity, Signature entitySignature) 
    {
        for (auto &pair : mSystems) 
        {
			auto const& type = pair.first;
			auto const& system = pair.second;
			auto const& systemSignature = mSignatures[type];
            
            // Si la firma de l'entity és igual a la del sistema ho afegim al set
			if ((entitySignature & systemSignature) == systemSignature)
			{
				system->mEntities.insert(entity);
			}
			// Si no coincideix ho eliminam del set
			else
			{
				system->mEntities.erase(entity);
			}

        }
    }

}

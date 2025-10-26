#include "ecs/EntityManager.hpp"

namespace ecs 
{
    
    EntityManager::EntityManager() 
    {
        // Inicialitzem el vector amb tots els possibles ID
        for (Entity entity = 0; entity < MAX_ENTITIES; ++entity) 
        {
            mAvailableEntitites.push(entity);    
        }
    }

    Entity EntityManager::createEntity() 
    {
        assert(mLivingEntityCount < MAX_ENTITIES && "Too many entities in existance");
        
        // Ara agafem una ID del front de la cua
        Entity id = mAvailableEntitites.front();
        mAvailableEntitites.pop();
        ++mLivingEntityCount;

        return id;
    }

    void EntityManager::destroyEntity(Entity entity) 
    {
        assert(entity < MAX_ENTITIES && "Entity out of range.");

        // Invalidam la firma de l'entitat destruida
        mSignatures[entity].reset();

        // Retornem la id destruida a la cua
        mAvailableEntitites.push(entity);
        --mLivingEntityCount;
        
    }

    void EntityManager::setSignature(Entity entity, Signature signature)
    {
        assert(entity < MAX_ENTITIES && "Entity out of range.");

		// Posem la firma de l'entitat a l'array
		mSignatures[entity] = signature;

    }

    Signature EntityManager::getSignature(Entity entity) const 
    {
        assert(entity < MAX_ENTITIES && "Entity out of range.");

		// Obtenim la firma de l'entitat de l'array
		return mSignatures[entity];

    }

    std::uint32_t EntityManager::getEntityLivingCount() const 
    {
        return mLivingEntityCount;
    }
}
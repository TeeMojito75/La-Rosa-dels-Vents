#include "ECS/EntityManager.hpp"

namespace ecs {
    
    EntityManager::EntityManager() {
        reset();
    }

    void EntityManager::reset() {
        count = 0;

        //Omplim la llista de entitats lliures amb tots els possibles índex en ordre invers
        for (Entity e = 0; e < MAX_ENTITIES; e++) {
            entitatsLliures[e] = MAX_ENTITIES - 1 - e;
            signatures[e] = 0;
        }
        headLliure = 0;
    }

    Entity EntityManager::create() {
        Entity index = entitatsLliures[headLliure++];
        count++;
        signatures[index] = 0;
        return index;
    }

    void EntityManager::destroy(Entity e) {
        // Netejem la signature i retorna l'índex de la llista d'entitats lliures
        signatures[e] = 0;
        entitatsLliures[headLliure--] = e;
        count--;
    }

    Signature EntityManager::getSignature(Entity e) const {
        return signatures[e];
    }
}
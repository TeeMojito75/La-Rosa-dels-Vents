#ifndef ECS_ENTITYMANAGER_HPP
#define ECS_ENTITYMANAGER_HPP

#include <stdint.h>
#include "ECS/Utils.hpp"

namespace ecs {

    class EntityManager {
        
         
        private:
            Entity entitatsLliures[MAX_ENTITIES];
            uint16_t headLliure = 0;
            Signature signatures[MAX_ENTITIES];
            uint16_t count = 0;

        public:
            EntityManager();

            void reset();

            // Contructora i destructora
            Entity create();
            void destroy(Entity e);

            // Signature (quin components té una entitat)
            void setSignature(Entity e, Signature s);
            Signature getSignature(Entity e) const;

            uint16_t size() const {
                return count;
            }
    };    
}

#endif
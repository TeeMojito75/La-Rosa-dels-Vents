#ifndef ECS_ENTITY_HPP
#define ECS_ENTITY_HPP

#include <cassert>
#include <limits>
#include "ecs/Utils.hpp"

namespace ecs {

class EntityManager; // forward

    class Entity
    {
        public:
            using Id = EntityId;
            static constexpr Id kInvalid = std::numeric_limits<Id>::max();

            Entity() : id_(kInvalid), mgr_(nullptr) {}

            Entity(Id id, EntityManager* mgr) : id_(id), mgr_(mgr)
            {
                assert(mgr != nullptr && "Entity must have a valid manager!");
            }

            Id id() const { return id_; }
            bool isValid() const { return mgr_ != nullptr && id_ != kInvalid; }
            
            bool operator==(const Entity& other) const
            {
                return id_ == other.id_;
            }

            bool operator!=(const Entity& other) const
            {
                return id_ != other.id_;
            }

            // API de les components
            template<typename T>
            void addComponent(const T& component);

            template<typename T>
            void removeComponent();

            template<typename T>
            T& getComponent();

            // Destrucció entitats
            void destroy();

        private:
            Id id_;
            EntityManager* mgr_;
    };
}

#endif

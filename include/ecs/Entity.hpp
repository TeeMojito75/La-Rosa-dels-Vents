#ifndef ECS_ENTITY_HPP
#define ECS_ENTITY_HPP
#include "ecs/Utils.hpp"

namespace ecs 
{
    class EntityManager;
    class Entity 
    {
        public:
        using Id = EntityId;
        static constexpr Id kInvalid = 0xFFFFFFFFu;
        
        Entity() : id_(kInvalid), mgr_(nullptr) {}
        explicit Entity(Id id, EntityManager* mgr) : id_(id), mgr_(mgr) {}
        
        Id id() const { return id_; }
        bool valid() const;
        
        bool operator==(const Entity& o) const { return id_ == o.id_; }
        bool operator!=(const Entity& o) const { return id_ != o.id_; }
        bool operator<(const Entity& o)  const { return id_  < o.id_; }
        
        private:
        Id id_;
        EntityManager* mgr_;
        friend class EntityManager;
    };

    struct EntityHash {
        std::size_t operator()(const Entity& e) const noexcept {
            return std::hash<Entity::Id>{}(e.id());
        }
    };

}
#endif
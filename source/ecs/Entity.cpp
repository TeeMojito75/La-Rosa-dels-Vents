#include "ecs/Entity.hpp"
#include "ecs/EntityManager.hpp"

namespace ecs 
{
    template<typename T>
    void Entity::addComponent(const T& component)
    {
        assert(isValid());
        mgr_->AddComponent<T>(*this, component);
    }

    template<typename T>
    void Entity::removeComponent()
    {
        assert(isValid());
        mgr_->RemoveComponent<T>(*this);
    }

    template<typename T>
    T& Entity::getComponent()
    {
        assert(isValid());
        return mgr_->GetComponent<T>(*this);
    }

    void Entity::destroy()
    {
        assert(isValid());
        mgr_->DestroyEntity(*this);
    }
}

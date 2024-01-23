#pragma once

#include <unordered_map>
#include <typeindex>

namespace raid
{

   class Component;

class Entity
{
public:

    template <typename T, typename... Args>
    T& AddComponent(Args&&... args)
    {
        T* component = new T(*this, std::forward<Args>(args)...);
        m_Components[typeid(T)] = component;
        return *component;
    }

    template <typename T>
    T* GetComponent() const
    {
        auto it = m_Components.find(typeid(T));
        return (it != m_Components.end()) ? static_cast<T*>(it->second) : nullptr;
    }

private:

    std::unordered_map<std::type_index, Component*> m_Components;
};

} // namespace raid
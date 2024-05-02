#pragma once

#include <cstddef>
#include <map>
#include <stdexcept>
#include <type_traits>

namespace TotoEngine {

namespace Core {

template <typename TYPE>
class Manager {
public:
    struct Instance {
        size_t id;
        operator size_t() const { return id; }
        operator TYPE&() const { return get(*this); }
        TYPE* operator->() const { return &get(*this); }
        TYPE& operator*() const { return get(*this); }
    };

    constexpr static auto NO_INSTANCE = Instance {0};

    static Instance create(TYPE&& instance = TYPE()) {
        auto id = nextInstance();
        getManager()._instances.emplace(id, std::move(instance));
        return id;
    }
    static void destroy(const Instance& id) {
        if(!getManager()._instances.contains(id))
            return;
        if constexpr (std::is_pointer<TYPE>::value) {
            delete getManager()._object_instances.at(id);
        }
        getManager()._instances.erase(id);
    }
    static TYPE& get(const Instance& id) {
        if(id == NO_INSTANCE)
            throw std::out_of_range("Instance points to null value");
        if(!getManager()._instances.contains(id))
            throw std::out_of_range("Instance points to non-existing object");
        return getManager()._instances.at(id);
    }

    static Manager& getManager() {
        static Manager manager;
        return manager;
    }
    Manager(const Manager&) = delete;
    Manager& operator=(const Manager&) = delete;
private:
    Manager() = default;
    ~Manager() = default;

    std::map<Instance, TYPE> _instances {};
    static Instance nextInstance() {
        static size_t id = 1;
        return Instance { id++ };
    }
    // size_t _unique_counter {1};
};

}

}

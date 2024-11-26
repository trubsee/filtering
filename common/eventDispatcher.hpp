#pragma once
#include <functional>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace Common {

class EventDispatcher {
    using EventHandlerBase = std::function<void(void*)>;

   public:
    template <typename T>
    void RegisterHandler(std::function<void(const T&)> handler) {
        EventHandlerBase wrapper = [handler](void* event) {
            handler(*static_cast<T*>(event));
        };
        mHandlers[typeid(T)].push_back(wrapper);
    }

    template <typename T>
    void Dispatch(const T& event) const {
        auto it = mHandlers.find(typeid(T));
        if (it != mHandlers.end()) {
            for (const auto& handler : it->second)
                handler(const_cast<T*>(&event));
        }
    }

   private:
    std::unordered_map<std::type_index, std::vector<EventHandlerBase> >
        mHandlers;
};

}  // namespace Common

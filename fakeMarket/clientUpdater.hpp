#pragma once
#include <functional>
#include <unordered_map>

#include "common/assert.hpp"
#include "common/eventDispatcher.hpp"
#include "fakeMarket/IClient.hpp"
#include "fakeMarket/events.hpp"

namespace FakeMarket {

class ClientUpdater {
   public:
    ClientUpdater(Common::EventDispatcher& eventDispatcher)
        : mEventDispatcher{eventDispatcher} {}

    void ConnectClient(IClient& client) {
        auto inserted = mClients.emplace(client.GetClientId(), client);
        ASSERT(inserted.second);
    }

    void SendResponse(ClientId id, const Response& msg) {
        GetClient(id).SendResponse(msg);
    }

    void SendFillPrivate(ClientId id, const PrivateFill& msg) {
        GetClient(id).FillPrivate(msg);
    }

    void SendTradePublic() {
        // mEventDispatcher.Dispatch();
    }

    void SendTOBPublic() {
        // mEventDispatcher.Dispatch();
    }

   private:
    IClient& GetClient(ClientId id) {
        const auto it = mClients.find(id);
        ASSERT(it != mClients.end());
        return it->second.get();
    }

    Common::EventDispatcher& mEventDispatcher;
    std::unordered_map<std::uint16_t, std::reference_wrapper<IClient>> mClients;
};

}  // namespace FakeMarket

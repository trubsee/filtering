#pragma once
#include <functional>
#include <unordered_map>

#include "common/assert.hpp"
#include "common/eventDispatcher.hpp"
#include "fakeMarket/events.hpp"
#include "fakeMarket/client.hpp"

namespace FakeMarket {

class ClientUpdater
{
public:
    ClientUpdater(Common::EventDispatcher& eventDispatcher)
    :
        mEventDispatcher{eventDispatcher}
    {
    }

    void ConnectClient(IClient& client)
    {
        auto inserted = mClients.emplace(client.GetClientId(), client);
        ASSERT(inserted.second);
    }

    void SendFAKResponse(ClientId id, const ResponseFAK& msg)
    {
        GetClient(id).FAKResponse(msg);
    }

    void SendQuoteUpdateResponse(ClientId id, const ResponseQuoteUpdate& msg)
    {
        GetClient(id).QuoteUpdateResponse(msg);
    }

    void SendQuoteDeleteResponse(ClientId id, const ResponseQuoteDelete& msg)
    {
        GetClient(id).QuoteDeleteResponse(msg);
    }

    void SendQuoteFillPrivate(ClientId id, const PrivateQuoteFill& msg)
    {
        GetClient(id).QuoteFillPrivate(msg);
    }

    void SendTradePublic()
    {
        // mEventDispatcher.Dispatch();
    }

    void SendTOBPublic()
    {
        // mEventDispatcher.Dispatch();
    }

private:
    IClient& GetClient(ClientId id)
    {
        const auto it = mClients.find(id);
        ASSERT(it != mClients.end());
        return it->second.get();
    }

    Common::EventDispatcher& mEventDispatcher;
    std::unordered_map<std::uint16_t, std::reference_wrapper<IClient>> mClients;
};

}

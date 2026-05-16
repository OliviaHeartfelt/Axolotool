#pragma once

#include "detail/ACallbackBase.h"

namespace ACallback {
    using Empty = ACallbackBase::Callback<>;

    template<class... Args_>
    using Args = ACallbackBase::Callback<Args_...>;

    template<ACallbackBase::IsQEventPtr T>
    using Event = ACallbackBase::EventCallback<T>;

    template<ACallbackBase::IsQEventPtr T, class... Args>
    using EventArgs = ACallbackBase::EventArgsCallback <T, Args...>;
}
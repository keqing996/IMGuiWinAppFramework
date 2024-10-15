#include "ImApp/Component/Component.h"

namespace IMWinApp
{
    void Component::UpdateView()
    {
        if (!_enable)
            return;

        Tick();
    }

    void Component::SetEnable(bool enable)
    {
        _enable = enable;
    }

    bool Component::Enable() const
    {
        return _enable;
    }
}

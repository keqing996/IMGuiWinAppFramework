#pragma once

#include "ComponentContainer.h"

namespace IMWinApp
{
    class HorizontalLayout: ComponentContainer
    {
    protected:
        void InternalTick() override;
    };
}
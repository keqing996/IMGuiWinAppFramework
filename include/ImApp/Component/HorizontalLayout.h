#pragma once

#include "ComponentContainer.h"

namespace IMWinApp
{
    class HorizontalLayout: public ComponentContainer
    {
    protected:
        void InternalTick() override;
    };
}
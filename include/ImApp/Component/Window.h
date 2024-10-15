#pragma once

#include <imgui.h>
#include <string>

#include "ComponentContainer.h"

namespace IMWinApp
{
    class Window: public ComponentContainer
    {
    public:
        explicit Window(const std::string& name, ImGuiWindowFlags flags = 0);

        void PreTick() override;
        void PostTick() override;

    private:
        std::string _name;
        ImGuiWindowFlags _flags;
    };
}
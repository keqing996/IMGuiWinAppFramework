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

        void SetWindowFlags(ImGuiWindowFlags flags);
        ImGuiWindowFlags GetWindowFlags() const;

    private:
        std::string _name;
        ImGuiWindowFlags _flags;
    };
}
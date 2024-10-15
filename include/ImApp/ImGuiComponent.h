#pragma once

#include <ranges>
#include <vector>
#include <ranges>
#include "imgui.h"

namespace IMWinApp::Layout
{
    class Component
    {
    public:
        virtual void TickBegin() = 0;
        virtual void TickEnd() = 0;
    };

    class ComponentContainer: public Component
    {
    public:
        void AddComponent(Component* pComp)
        {
            if (pComp == nullptr)
                return;

            _children.push_back(pComp);
        }

        void RemoveComponent(Component* pComp)
        {
            if (pComp == nullptr)
                return;

            auto itr = std::ranges::find(_children, pComp);
            if (itr != _children.end())
                _children.erase(itr);
        }

        void TickBegin() override
        {
            for (auto& itr : _children)
                itr->TickBegin();
        }

        void TickEnd() override
        {
            for (auto& itr : std::ranges::reverse_view(_children))
                itr->TickEnd();
        }

    private:
        std::vector<Component*> _children;
    };

    class Window: public ComponentContainer
    {
    public:
        explicit Window(const std::string& name, ImGuiWindowFlags flags = 0)
            : _name(name)
            , _flags(flags)
        {
        }

        void TickBegin() override
        {
            ImGui::Begin(_name.c_str(), nullptr, _flags);
            ComponentContainer::TickBegin();
        }

        void TickEnd() override
        {
            ComponentContainer::TickEnd();
            ImGui::End();
        }

    private:
        std::string _name;
        ImGuiWindowFlags _flags;
    };


}
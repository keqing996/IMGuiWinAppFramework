#pragma once

#include <ranges>
#include <vector>
#include <list>
#include <memory>
#include <functional>
#include "imgui.h"

namespace IMWinApp::Layout
{




    class VerticalLayout: ComponentContainer
    {
    };

    class HorizontalLayout: ComponentContainer
    {
    protected:
        void InternalTick() override
        {
            ImGui::SameLine();
        }
    };

    class Window: public ComponentContainer
    {
    public:
        explicit Window(const std::string& name, ImGuiWindowFlags flags = 0)
            : _name(name)
            , _flags(flags)
        {
        }

        void PreTick() override
        {
            ImGui::Begin(_name.c_str(), nullptr, _flags);
        }

        void PostTick() override
        {
            ImGui::End();
        }

    private:
        std::string _name;
        ImGuiWindowFlags _flags;
    };

    class Button: public Component
    {
    public:
        Button(const std::string& name, const std::function<void(void)>& callback)
            : _name(name), _callback(callback)
        {
        }

        void Tick() override
        {
            if (ImGui::Button(_name.c_str()))
                _callback();
        }

    private:
        std::string _name;
        std::function<void(void)> _callback;
    };

    class Text: public Component
    {
    public:
        explicit Text(const std::string& content): _content(content) {}

        void SetText(const std::string& content)
        {
            _content = content;
        }

        void Tick() override
        {
            ImGui::Text(_content.c_str());
        }

    private:
        std::string _content;
    };
}
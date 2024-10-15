#pragma once

#include <ranges>
#include <vector>
#include <list>
#include <memory>
#include <functional>
#include "imgui.h"

namespace IMWinApp::Layout
{
    class Component
    {
    public:
        virtual void Tick() = 0;
    };

    class ComponentContainer: public Component
    {
        struct CompOperation
        {
            std::unique_ptr<Component> pComp;
            bool isAdd;
        };

    public:
        ComponentContainer() = default;

        ComponentContainer(std::initializer_list<Component*> initList)
        {

        }

        ComponentContainer& AddComponent(Component* pComp)
        {
            if (pComp == nullptr)
                return *this;

            if (_looping)
                _waitingComp.push_back(CompOperation{
                    std::unique_ptr<Component>(pComp),
                    true
                });
            else
                _children.push_back(std::unique_ptr<Component>(pComp));

            return *this;
        }

        void RemoveComponent(Component* pComp)
        {
            if (pComp == nullptr)
                return;

            if (_looping)
                _waitingComp.push_back(CompOperation{
                    std::unique_ptr<Component>(pComp),
                    false
                });
            else
            {
                auto itr = std::find(_children.begin(), _children.end(),
                    [pComp](const std::unique_ptr<Component>& uniquePtr) -> bool
                    {
                        return pComp == uniquePtr.get();
                    });
                if (itr != _children.end())
                    _children.erase(itr);
            }
        }

        void Tick() final
        {
            _looping = true;

            PreTick();

            for (int i = 0; i < _children.size(); i++)
            {
                _children[i]->Tick();
                if (i < _children.size() - 1)
                    InternalTick();
            }

            PostTick();

            _looping = false;

            FlushWaitingQueue();
        }

        bool IsLooping() const
        {
            return _looping;
        }

        const std::vector<Component*> GetChildren()
        {
            return _children;
        }

        virtual void PreTick()
        {
        }

        virtual void InternalTick()
        {
        }

        virtual void PostTick()
        {
        }

    private:
        void FlushWaitingQueue()
        {
            for (auto& compOp: _waitingComp)
            {
                if (compOp.isAdd)
                    _children.push_back(std::move(compOp.pComp));
                else
                    RemoveComponent(compOp.pComp);
            }

            _waitingComp.clear();
        }

    private:
        bool _looping = false;
        std::vector<std::unique_ptr<Component>> _children;
        std::vector<CompOperation> _waitingComp;
    };

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
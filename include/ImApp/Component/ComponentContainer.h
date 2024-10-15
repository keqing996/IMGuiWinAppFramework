#pragma once

#include <vector>
#include "Component.h"

namespace IMWinApp
{
    class ComponentContainer: public Component
    {
        struct CompOperation
        {
            Component* pComp;
            bool isAdd;
        };

    public:
        ComponentContainer() = default;
        ComponentContainer(std::initializer_list<Component*> initList);

    public:
        void Tick() final;
        virtual void PreTick();
        virtual void InternalTick();
        virtual void PostTick();

        ComponentContainer& AddComponent(Component* pComp);
        void RemoveComponent(Component* pComp);
        bool IsLooping() const;
        const std::vector<std::unique_ptr<Component>>& GetChildren();

    private:
        void FlushWaitingQueue();

    private:
        bool _looping = false;
        std::vector<std::unique_ptr<Component>> _children;
        std::vector<CompOperation> _waitingComp;
    };
}
#pragma once

#include <vector>
#include "Component.h"

namespace IMWinApp
{
    class ComponentContainer: public Component
    {
        struct CompOperation
        {
            std::unique_ptr<Component> pComp;
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
        ComponentContainer& AddComponent(std::unique_ptr<Component>&& pComp);
        void RemoveComponent(Component* pComp);
        bool IsLooping() const;
        const std::vector<Component*> GetChildren();

    private:
        void FlushWaitingQueue();

    private:
        bool _looping = false;
        std::vector<std::unique_ptr<Component>> _children;
        static std::vector<CompOperation> _waitingComp;
    };
}
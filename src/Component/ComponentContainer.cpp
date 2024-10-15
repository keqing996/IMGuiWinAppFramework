
#include <initializer_list>
#include <memory>
#include "ImApp/Component/ComponentContainer.h"

namespace IMWinApp
{
    ComponentContainer::ComponentContainer(std::initializer_list<Component*> initList)
    {

    }

    ComponentContainer& ComponentContainer::AddComponent(Component* pComp)
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

    ComponentContainer& ComponentContainer::AddComponent(std::unique_ptr<Component>&& pComp)
    {
        if (pComp == nullptr)
            return *this;

        if (_looping)
            _waitingComp.push_back(CompOperation{
                std::move(pComp),
                true
            });
        else
            _children.push_back(pComp);

        return *this;
    }

    void ComponentContainer::RemoveComponent(Component* pComp)
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

    void ComponentContainer::Tick()
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

    bool ComponentContainer::IsLooping() const
    {
        return _looping;
    }

    const std::vector<Component*> ComponentContainer::GetChildren()
    {
        return _children;
    }

    void ComponentContainer::PreTick()
    {
    }

    void ComponentContainer::InternalTick()
    {
    }

    void ComponentContainer::PostTick()
    {
    }

    void ComponentContainer::FlushWaitingQueue()
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
}

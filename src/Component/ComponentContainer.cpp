
#include <initializer_list>
#include <memory>
#include "ImApp/Component/ComponentContainer.h"

namespace IMWinApp
{
    ComponentContainer::ComponentContainer(std::initializer_list<Component*> initList)
    {
        for (auto pComp: initList)
            AddComponent(pComp);
    }

    ComponentContainer& ComponentContainer::AddComponent(Component* pComp)
    {
        if (pComp == nullptr)
            return *this;

        if (_looping)
            _waitingComp.push_back(CompOperation{
                pComp,
                true
            });
        else
            _children.push_back(std::unique_ptr<Component>(pComp));

        return *this;
    }

    void ComponentContainer::RemoveComponent(Component* pComp)
    {
        if (pComp == nullptr)
            return;

        if (_looping)
            _waitingComp.push_back(CompOperation{
                pComp,
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
            _children[i]->UpdateView();
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

    const std::vector<std::unique_ptr<Component>>& ComponentContainer::GetChildren()
    {
        return _children;
    }

    void ComponentContainer::PreTick()
    {
        // do nothing
    }

    void ComponentContainer::InternalTick()
    {
        // do nothing
    }

    void ComponentContainer::PostTick()
    {
        // do nothing
    }

    void ComponentContainer::FlushWaitingQueue()
    {
        for (auto& compOp: _waitingComp)
        {
            if (compOp.isAdd)
                AddComponent(compOp.pComp);
            else
                RemoveComponent(compOp.pComp);
        }

        _waitingComp.clear();
    }
}

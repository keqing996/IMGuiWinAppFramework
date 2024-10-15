#pragma once

namespace IMWinApp
{
    class Component
    {
    public:
        virtual ~Component() = default;

    public:
        void UpdateView();
        void SetEnable(bool enable);
        bool Enable() const;

    protected:
        virtual void Tick() = 0;

    private:
        bool _enable = true;
    };
}
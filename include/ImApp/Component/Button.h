#pragma once

#include <functional>
#include <string>
#include "Component.h"

namespace IMWinApp
{
    class Button: public Component
    {
    public:
        Button(const std::string& name, const std::function<void(void)>& callback);

        void SetText(const std::string& content);
        void Tick() override;

    private:
        std::string _name;
        std::function<void(void)> _callback;
    };
}
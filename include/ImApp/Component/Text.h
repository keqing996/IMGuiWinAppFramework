#pragma once

#include <string>
#include "Component.h"

namespace IMWinApp
{
    class Text: public Component
    {
    public:
        explicit Text(const std::string& content);

        void SetText(const std::string& content);

        void Tick() override;

    private:
        std::string _content;
    };
}
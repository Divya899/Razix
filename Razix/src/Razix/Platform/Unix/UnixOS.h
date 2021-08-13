#pragma once
#include "Core/OS/OS.h"

namespace Razix
{
    class UnixOS : public OS
    {
    public:
        UnixOS() = default;
        ~UnixOS() = default;

        void Init();
        void Run() override;
        std::string GetExecutablePath() override
        {
            return "";
        }
    };
}

#pragma once

#include <renderer/renderer.h>
#include <Window.h>

namespace Board
{
    enum class Action
    {
        LeftClick,
        RightClick
    };

    void Init(const glm::ivec2& boardSize, const glm::vec2& iwindowSize);
    void Shutdown();

    void Draw(Window& window);

    void Click(const glm::vec2& pos, Action action);

    bool GetFailStatus();
}
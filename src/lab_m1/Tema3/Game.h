#pragma once

#include "components/simple_scene.h"

#include "Common.h"
#include "Object/Object.h"

class Game : public gfxc::SimpleScene {
public:
    Game();
    ~Game();
    void Init() override;

private:
    void FrameStart() override;
    void Update(float deltaTimeSeconds) override;
    void FrameEnd() override;

    void OnInputUpdate(float deltaTime, int mods) override;
    void OnKeyPress(int key, int mods) override;
    void OnKeyRelease(int key, int mods) override;
    void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
    void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
    void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
    void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
    void OnWindowResize(int width, int height) override;

    Texture2D* CreateRandomTexture(unsigned int width, unsigned int height);
    float time = 0;
    int mode = 1;

public:
    std::vector<Object*> objects;
};

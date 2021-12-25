#include "Game.h"

Game::Game() {}
Game::~Game() {}

void Game::Init() {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
#pragma region assets
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("quad");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "quad.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }


#pragma endregion
#pragma region shaders
    {
        Shader* shader = new Shader("basic");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "Shaders", "Basic", "vert.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "Shaders", "Basic", "frag.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
#pragma endregion
}

void Game::Update(float deltaTimeSeconds) {}

void Game::OnInputUpdate(float deltaTime, int mods) {}

void Game::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY){}

void Game::OnWindowResize(int width, int height) {
    glm::vec2 resolution = window->GetResolution();
    GetSceneCamera()->SetPerspective(60, resolution.x / resolution.y, 0.01f, 400);
    GetSceneCamera()->Update();
}
void Game::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {}
void Game::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {}
void Game::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}
void Game::OnKeyPress(int key, int mods) {}
void Game::OnKeyRelease(int key, int mods) {}

void Game::FrameStart() {
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}
void Game::FrameEnd() {
    DrawCoordinateSystem(GetSceneCamera()->GetViewMatrix(), GetSceneCamera()->GetProjectionMatrix());
}


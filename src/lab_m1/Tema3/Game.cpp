#include "Game.h"
#include "components/transform.h"




void Game::Init() {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
    {
        Mesh* mesh = new Mesh("cone");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "cone.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("dancer1");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "Tema3"), "dancer1.obj");
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
#pragma region entities
    float wallsHeight = 5;

    // FLOOR
    {
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++) {
                FloorTile* o = new FloorTile();
                o->model.pos = glm::vec3(i - 3.5, 0, j - 3.5);
                o->model.rot = glm::quat(glm::vec3(glm::radians(90.f), 0, 0));
                o->model.scale = glm::vec3(0.5f);
                o->lightIndex = i * 8 + j;
                o->mesh = meshes["quad"];

               
                objects.push_back(o);
            }
    }

    // DISCO BALL
    {
        DiscoBall* o = new DiscoBall();
        o->texture = CreateRandomTexture(16, 16);
        o->mesh = meshes["sphere"];
        o->model.pos.y = wallsHeight - 0.5;
        objects.push_back(o);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, o->texture->GetTextureID());
        glUniform1i(glGetUniformLocation(shaders["basic"]->program, "discoBallTex"), 1);
    }
    
    // WALLS
    { 
        // front // o->lightIndex % 8 == 7
        // back // o->lightIndex % 8 == 0
        // left // o->lightIndex < 8
        // right // o->lightIndex >= 8 * 7
        { // right
            Wall* o = new Wall();
            o->model.pos = { 4, wallsHeight / 2, 0 };
            o->model.rot = glm::quat(glm::vec3(0, glm::radians(90.f), 0));
            o->model.scale = glm::vec3(4, wallsHeight / 2, 4);
            o->mesh = meshes["quad"];
            for (int i = 0; i < 64; i++)
                if (((FloorTile*)objects[i])->lightIndex >= 8 * 7)
                    o->closestTiles.push_back((LightSource*)objects[i]);
            objects.push_back(o);
        }

        { // left
            Wall* o = new Wall();
            o->model.pos = { -4, wallsHeight / 2, 0 };
            o->model.rot = glm::quat(glm::vec3(0, glm::radians(-90.f), 0));
            o->model.scale = glm::vec3(4, wallsHeight / 2, 4);
            o->mesh = meshes["quad"];
            for (int i = 0; i < 64; i++)
                if (((FloorTile*)objects[i])->lightIndex < 8)
                    o->closestTiles.push_back((LightSource*)objects[i]);
            objects.push_back(o);
        }
        { // front
            Wall* o = new Wall();
            o->model.pos = { 0, wallsHeight / 2, 4 };
            o->model.scale = glm::vec3(4, wallsHeight / 2, 4);
            o->mesh = meshes["quad"];
            for (int i = 0; i < 64; i++)
                if (((FloorTile*)objects[i])->lightIndex % 8 == 7)
                    o->closestTiles.push_back((LightSource*)objects[i]);
            objects.push_back(o);
        }
        { // back
            Wall* o = new Wall();
            o->model.pos = { 0, wallsHeight / 2, -4 };
            o->model.rot = glm::quat(glm::vec3(0, glm::radians(180.f), 0));
            o->model.scale = glm::vec3(4, wallsHeight / 2, 4);
            o->mesh = meshes["quad"];
            for (int i = 0; i < 64; i++)
                if (((FloorTile*)objects[i])->lightIndex % 8 == 0)
                    o->closestTiles.push_back((LightSource*)objects[i]);
            objects.push_back(o);
        }
        { // top
            Wall* o = new Wall();
            o->model.pos = { 0, wallsHeight, 0};
            o->model.rot = glm::quat(glm::vec3(glm::radians(-90.f), 0, 0));
            o->model.scale = glm::vec3(4, 4, 1);
            o->mesh = meshes["quad"];
            objects.push_back(o);
        }
    }

   
    // DANCERS
    float dancers_height = 1.f;
    {
        {
            Dancer* o = new Dancer;
            o->model.scale = glm::vec3(0.5, 1, 0.5);
            o->model.pos.y = dancers_height / 2;
            o->mesh = meshes["box"];
            o->game_assets = &objects;
            objects.push_back(o);
        }
    
        {
            Dancer* o = new Dancer;
            o->model.scale = glm::vec3(0.5, 1, 0.5);
            o->model.pos.y = dancers_height / 2;
            o->mesh = meshes["box"];
            o->game_assets = &objects;
            objects.push_back(o);
        }

        {
            Dancer* o = new Dancer;
            o->model.scale = glm::vec3(0.5, 1, 0.5);
            o->model.pos.y = dancers_height / 2;
            o->mesh = meshes["box"];
            o->game_assets = &objects;
            objects.push_back(o);
        }

    }
    
    // SPOTLIGHTS
    {
        {
            Spotlight* o = new Spotlight({ -2, wallsHeight, -2 });
            o->mesh = meshes["cone"];
            o->model.scale = glm::vec3(1, wallsHeight, 1);
            o->spotlight_index = 0;
            objects.push_back(o);
        }
        {
            Spotlight* o = new Spotlight({ 2, wallsHeight, -2 });
            o->mesh = meshes["cone"];
            o->model.scale = glm::vec3(1, wallsHeight, 1);
            o->spotlight_index = 1;
            objects.push_back(o);
        }
        {
            Spotlight* o = new Spotlight({ -2, wallsHeight, 2 });
            o->mesh = meshes["cone"];
            o->model.scale = glm::vec3(1, wallsHeight, 1);
            o->spotlight_index = 2;
            objects.push_back(o);
        }
        {
            Spotlight* o = new Spotlight({ 2, wallsHeight, 2 });
            o->mesh = meshes["cone"];
            o->model.scale = glm::vec3(1, wallsHeight, 1);
            o->spotlight_index = 3;
            objects.push_back(o);
        }
    }
#pragma endregion

    GetSceneCamera()->SetPosition(glm::vec3(0, 3, 10));
}

void Game::Update(float deltaTimeSeconds) {
    auto shader = shaders["basic"];
    
    shader->Use();

    auto eye = GetSceneCamera()->m_transform->GetWorldPosition();
    glUniform1i(shader->GetUniformLocation("mode"), mode);
    glUniform1f(shader->GetUniformLocation("time"), time);
    glUniform3f(shader->GetUniformLocation("eye"), eye.x, eye.y, eye.z);
    glUniformMatrix4fv(shader->GetUniformLocation("View"), 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetViewMatrix()));
    glUniformMatrix4fv(shader->GetUniformLocation("Proj"), 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetProjectionMatrix()));

    for (auto obj : objects) {
        obj->render(shader);
        obj->update(deltaTimeSeconds);
    }
    
    time += deltaTimeSeconds;
}

void Game::OnKeyPress(int key, int mods) {
    if (window->KeyHold(GLFW_KEY_SPACE)) {
        mode++;
        mode %= 4;
    }
}

void Game::OnWindowResize(int width, int height) {
    glm::vec2 resolution = window->GetResolution();
    GetSceneCamera()->SetPerspective(60, resolution.x / resolution.y, 0.01f, 400);
    GetSceneCamera()->Update();
}
Texture2D* Game::CreateRandomTexture(unsigned int width, unsigned int height) {
    GLuint textureID = 0;
    unsigned int channels = 3;
    unsigned int size = width * height * channels;
    unsigned char* data = new unsigned char[size];

    // Generate random texture data
    for (int i = 0; i < size; i++)
        data[i] = rand();

    // Generate and bind the new texture ID
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    if (GLEW_EXT_texture_filter_anisotropic) {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
    }
    // Set the texture parameters (MIN_FILTER, MAG_FILTER and WRAPPING MODE) using glTexParameteri
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    CheckOpenGLError();

    // Use glTexImage2D to set the texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    // Generate texture mip-maps
    glGenerateMipmap(GL_TEXTURE_2D);

    CheckOpenGLError();

    // Save the texture into a wrapper Texture2D class for using easier later during rendering phase
    Texture2D* texture = new Texture2D();
    texture->Init(textureID, width, height, channels);

    SAFE_FREE_ARRAY(data);
    return texture;
}
void Game::FrameStart() {
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

Game::Game() {}
Game::~Game() {
    for (auto& obj : objects)
        delete obj;
}

void Game::FrameEnd() { /* DrawCoordinateSystem(GetSceneCamera()->GetViewMatrix(), GetSceneCamera()->GetProjectionMatrix()); */ }
void Game::OnInputUpdate(float deltaTime, int mods) {}
void Game::OnKeyRelease(int key, int mods) {}
void Game::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {}
void Game::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {}
void Game::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {}
void Game::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}
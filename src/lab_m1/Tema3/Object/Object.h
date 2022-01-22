#define GLM_SWIZZLE 
#define GLM_SWIZZLE_XYZW 
#define GLM_SWIZZLE_STQP
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "components/simple_scene.h"

glm::vec3 generateRandomColor();

class Transform {
public:
	glm::vec3 
		pos = glm::vec3(0), 
		scale = glm::vec3(1);
	glm::quat rot = glm::quat(glm::vec3(0));
	glm::mat4 toMat4();
};





class Object {
public:
	Transform model;
	Mesh* mesh = nullptr;
	int type = -1;

	virtual void render(Shader* s) = 0;
	virtual void update(float dt) = 0;
};




class LightSource : public Object {
public:
	glm::vec3 color = glm::vec3(1);
};





class FloorTile : public LightSource {
public:
	glm::vec3 next_color;
	int lightIndex;
	FloorTile();
	void render(Shader* s) override;
	void update(float dt) override;
};





class Wall : public LightSource {
public:
	std::vector<LightSource*> closestTiles;
	Wall();
	void render(Shader* s) override;
	void update(float dt) override;
};






class DiscoBall : public Object {
public:
	Texture2D* texture;
	DiscoBall();
	void render(Shader* s) override;
	void update(float dt) override;
};




class Dancer : public Object {
public:
	glm::vec3 start_pos;
	glm::vec3 next_pos;
	bool jumping;
	glm::vec3 generateRandomPosition();
	std::vector<Object*> *game_assets;
	Dancer();
	void render(Shader* s) override;
	void update(float dt) override;
};




class Spotlight : public Object {
public:
	glm::vec3 color, next_color, dir, next_dir;
	int spotlight_index;
	Spotlight(glm::vec3 pos);
	void render(Shader* s) override;
	void update(float dt) override;
	glm::vec3 generateRandomDir();
};


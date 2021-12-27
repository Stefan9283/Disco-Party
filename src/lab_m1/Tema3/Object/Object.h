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
	Mesh* mesh;
	int type;

	virtual void render(Shader* s) = 0;
	virtual void update(float dt) = 0;
};

class FloorTile : public Object {
public:
	glm::vec3 color, next_color;
	int lightIndex;
	FloorTile();
	void render(Shader* s) override;
	void update(float dt) override;
};

class Wall : public Object {
public:
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
	glm::vec3 next_pos;
	glm::vec3 generateRandomPosition();
	Dancer();
	void render(Shader* s) override;
	void update(float dt) override;
};

class Spotlight : public Object {
public:
	glm::vec3 color, next_color, dir, next_dir; // TODO use them
	int spotlight_index;
	Spotlight();
	void render(Shader* s) override;
	void update(float dt) override;
	glm::vec3 generateRandomDir();
};


#include "Object.h"
#include "../Common.h"

class Game;

#define FLOOR 1
#define DISCOBALL 2
#define DANCER 3
#define WALL 4
#define SPOTLIGHT 5

glm::mat4 Transform::toMat4() {
	return glm::translate(glm::mat4(1), pos) *
		glm::toMat4(rot) *
		glm::scale(glm::mat4(1), scale);
}
glm::vec3 generateRandomColor() {
	float r, g, b;
	r = rand() % 256;
	g = rand() % 256;
	b = rand() % 256;
	return glm::vec3(r, g, b) / 255.f;
}
glm::vec3 Dancer::generateRandomPosition() {
	float x = rand() % 70 - 30;
	float z = rand() % 70 - 30;
	return glm::vec3(x, 0, z) / 10.f;
}






FloorTile::FloorTile() {
	type = FLOOR;
	color = generateRandomColor();
	next_color = generateRandomColor();
}
void FloorTile::render(Shader* s) {
	glUniform1i(s->GetUniformLocation("type"), type);
	glUniformMatrix4fv(s->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(this->model.toMat4()));
	glUniform3f(s->GetUniformLocation(std::string("lights[0].color").c_str()), color.x, color.y, color.z);
	glUniform3f(s->GetUniformLocation(std::string("lights[0].position").c_str()), model.pos.x, model.pos.y, model.pos.z);

	mesh->Render();
}
void FloorTile::update(float dt) {
	if (glm::distance(color, next_color) <= 0.01) {
		next_color = generateRandomColor();
	}
	else {
		color += glm::normalize(next_color - color) * 0.005f;
	}
}










Wall::Wall(){
	type = WALL;
}
void Wall::render(Shader* s) {
	glUniform1i(s->GetUniformLocation("type"), type);

	for (int i = 0; i < 9; i++) {
		glUniform3f(s->GetUniformLocation(std::string("lights[").append(std::to_string(i)).append("].color").c_str()), 0, 0, 0);
		glUniform3f(s->GetUniformLocation(std::string("lights[").append(std::to_string(i)).append("].position").c_str()), 0, 100, 0);
	}

	for (int i = 0; i < closestTiles.size(); i++) {
		glm::vec3 color = closestTiles[i]->color;
		glm::vec3 pos = closestTiles[i]->model.pos;
		glUniform3f(s->GetUniformLocation(std::string("lights[").append(std::to_string(i)).append("].color").c_str()), color.x, color.y, color.z);
		glUniform3f(s->GetUniformLocation(std::string("lights[").append(std::to_string(i)).append("].position").c_str()), pos.x, pos.y, pos.z);
	}
	
	glUniformMatrix4fv(s->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(this->model.toMat4()));
	mesh->Render();
}
void Wall::update(float dt) { /* DOES NOTHING */ }





DiscoBall::DiscoBall() {
	type = DISCOBALL;
	texture = nullptr;
}
void DiscoBall::render(Shader* s) {
	glUniform1i(s->GetUniformLocation("type"), type);
	glUniform3f(s->GetUniformLocation("disco_ball_position"), model.pos.x, model.pos.y, model.pos.z);
	glUniformMatrix4fv(s->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(this->model.toMat4()));
	mesh->Render();
}
void DiscoBall::update(float dt) {/* NOTHING */ }




Dancer::Dancer() {
	type = DANCER;
	next_pos = generateRandomPosition();
	start_pos = model.pos;
	jumping = true;
}
void Dancer::render(Shader* s) {
	glUniform1i(s->GetUniformLocation("type"), type);

	std::vector<FloorTile*> closest;
	for (int i = 0; i < 64; i++) {
		auto tile = ((FloorTile*)(*game_assets)[i]);
		if (glm::distance(tile->model.pos, this->model.pos) < 1.5) 
			closest.push_back(tile);
	}
	
	for (int i = 0; i < closest.size(); i++) {
		glm::vec3 color = closest[i]->color;
		glm::vec3 pos = closest[i]->model.pos;
		glUniform3f(s->GetUniformLocation(std::string("lights[").append(std::to_string(i)).append("].color").c_str()), color.x, color.y, color.z);
		glUniform3f(s->GetUniformLocation(std::string("lights[").append(std::to_string(i)).append("].position").c_str()), pos.x, pos.y, pos.z);
	}

	glUniformMatrix4fv(s->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(this->model.toMat4()));
	mesh->Render();
}
void Dancer::update(float dt) {
	if (glm::length(model.pos.xz - next_pos.xz) < 0.05) {
		start_pos = model.pos;
		next_pos = generateRandomPosition();
		jumping = false;
		if (rand() % 5 == 0) jumping = true;
	} else {
		auto pos = model.pos;
		pos.y = 0;

		glm::vec3 dir = glm::normalize(next_pos - pos);
		model.pos += dir * 0.01f;
		if (jumping) {
			auto l1 = glm::length(model.pos.xz - next_pos.xz);
			auto l0 = glm::length(start_pos.xz - next_pos.xz);
			float ratio = 1 - l1 / l0;
			float max_height = 2;
			if (ratio < 0.5f)
				model.pos.y = 0.5f + glm::pow(ratio, 2) * max_height;
			else
				model.pos.y = 0.5f + glm::pow(1 - ratio, 2) * max_height;
		}
	}
}






Spotlight::Spotlight(glm::vec3 pos) {
	type = SPOTLIGHT;
	
	model.pos = pos;
	model.rot = glm::quat(glm::vec3(0, 0, 0));

	color = generateRandomColor();
	next_color = color;
	
	dir = glm::vec3(0, -1, 0);
	next_dir = generateRandomDir();
}
void Spotlight::render(Shader* s) {
	glUniform1i(s->GetUniformLocation("type"), type);
	glUniform3f(s->GetUniformLocation(std::string("spotlights[").append(std::to_string(spotlight_index)).append("].color").c_str()), color.x, color.y, color.z);
	glUniform3f(s->GetUniformLocation(std::string("spotlights[").append(std::to_string(spotlight_index)).append("].position").c_str()), model.pos.x, model.pos.y, model.pos.z);
	glUniform3f(s->GetUniformLocation(std::string("spotlights[").append(std::to_string(spotlight_index)).append("].direction").c_str()), dir.x, dir.y, dir.z);
	glUniform1f(s->GetUniformLocation(std::string("spotlights[").append(std::to_string(spotlight_index)).append("].cutoff").c_str()), 8.f);
	glUniform1i(s->GetUniformLocation("idx"), spotlight_index);

	glUniformMatrix4fv(s->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(this->model.toMat4()));
	mesh->Render();
}
void Spotlight::update(float dt) {
	if (glm::distance(color, next_color) <= 0.05) {
		next_color = generateRandomColor();
	} else {
		color += glm::normalize(next_color - color) * 0.01f;
	}

	if (glm::distance(dir, next_dir) <= 0.01) {
		next_dir = generateRandomDir();
	} else {
		glm::vec3 target = glm::normalize(dir + 0.005f * glm::normalize(next_dir - dir));
		model.rot = glm::normalize(glm::rotation(glm::vec3(0, -1, 0), target));
		dir = target;
	}
}
glm::vec3 Spotlight::generateRandomDir() {
	float x = rand() % 900 - 450;
	float z = rand() % 900 - 450;
	glm::vec3 floorPos = glm::vec3(x, -600, z) / 100.f;
	auto newdir = glm::normalize(floorPos - model.pos);
	return newdir;
}
#include "Object.h"
#include "../Common.h"

#define FLOOR 1
#define DISCOBALL 2
#define DANCER 3
#define WALL 4

glm::mat4 Transform::toMat4() {
	return glm::translate(glm::mat4(1), pos) *
		glm::toMat4(rot) *
		glm::scale(glm::mat4(1), scale);
}

FloorTile::FloorTile() {
	type = FLOOR;
	color = generateRandomColor();
	next_color = generateRandomColor();
}

void FloorTile::render(Shader* s) {
	// TODO
	glUniform1i(s->GetUniformLocation("tileIdx"), lightIndex);
	glUniform1i(s->GetUniformLocation("type"), type);
	glUniformMatrix4fv(s->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(this->model.toMat4()));
	glUniform3f(s->GetUniformLocation(std::string("lights[").append(std::to_string(lightIndex)).append("].color").c_str()), color.x, color.y, color.z);
	glUniform3f(s->GetUniformLocation(std::string("lights[").append(std::to_string(lightIndex)).append("].position").c_str()), model.pos.x, model.pos.y, model.pos.z);

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

glm::vec3 generateRandomColor() {
	float r, g, b;
	r = rand() % 256;
	g = rand() % 256;
	b = rand() % 256;
	return glm::vec3(r, g, b) / 255.f;
}

Wall::Wall(){
	type = WALL;
}

void Wall::render(Shader* s) {
	glUniform1i(s->GetUniformLocation("type"), type);
	glUniformMatrix4fv(s->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(this->model.toMat4()));
	mesh->Render();
}

void Wall::update(float dt) { /* DOES NOTHING */ }

DiscoBall::DiscoBall() {
	type = DISCOBALL;
}

void DiscoBall::render(Shader* s) {
	// TODO
	glUniform1i(s->GetUniformLocation("type"), type);
	glUniform3f(s->GetUniformLocation("disco_ball_position"), model.pos.x, model.pos.y, model.pos.z);
	glUniformMatrix4fv(s->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(this->model.toMat4()));
	mesh->Render();
}

void DiscoBall::update(float dt) {/* NOTHING */ }

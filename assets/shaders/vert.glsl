#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;

// Uniform properties
uniform mat4 Global;
uniform mat4 Local;
uniform mat4 View;
uniform mat4 Proj;
uniform vec3 Color; // TODO use

out vec3 fragColor;

void main()
{
    fragColor = v_normal;
    gl_Position = Proj * View * Global * Local * vec4(v_position, 1.0);
}

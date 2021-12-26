#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 View;
uniform mat4 Proj;
uniform mat4 Model;

// Output
out vec3 normal;
out vec3 fragpos;

void main()
{
    normal = normalize(mat3(Model) * v_normal);
    fragpos = (Model * vec4(v_position, 1.0)).xyz;
    gl_Position = Proj * View * vec4(fragpos, 1);
}

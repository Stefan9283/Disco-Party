#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 3) in mat4 instanceMatrix;

// Uniform properties
uniform mat4 View;
uniform mat4 Proj;

// Output
out vec3 normal;
out vec3 fragpos;

void main()
{
    normal = normalize(mat3(instanceMatrix) * v_normal);
    fragpos = (instanceMatrix * vec4(v_position, 1.0)).xyz;
    gl_Position = Proj * View * vec4(fragpos, 1);
}

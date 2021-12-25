#version 330

// Input
in vec3 frag_color; // TODO use

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    out_color = vec4(vec3(1), 1);
}

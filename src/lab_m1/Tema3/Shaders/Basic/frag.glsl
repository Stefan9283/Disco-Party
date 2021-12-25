#version 330

// Input
in vec3 normal;
in vec3 fragpos;

// Output
layout(location = 0) out vec4 out_color;

// Uniforms
uniform vec3 Color;

void main()
{
    vec3 light_pos = vec3(0, 10, 0);
    vec3 light_dir = light_pos - fragpos;

    float intensity = max(dot(light_dir, normal), 0) / length(light_dir);

	vec3 color = Color;

	if (intensity > 0.95)
		color *= 0.8;
	else if (intensity > 0.75)
		color *= 0.6;
	else if (intensity > 0.5)
		color *= 0.4;
	else
		color *= 0.3;

    out_color = vec4(color, 1);
}

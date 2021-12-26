#version 330

// Output
layout(location = 0) out vec4 out_color;

// Input
in vec3 normal;
in vec3 fragpos;


// Uniforms
#define Floor 1
#define DiscoBall 2
#define Dancer 3
#define Wall 4

struct lightSource {
    vec3 color;
    vec3 position;
};


uniform vec3 eye;
uniform float time;

uniform int type;

uniform int tileIdx; // only if type == Floor
uniform lightSource lights[100]; // the first 64 one are the tiles

uniform vec3 disco_ball_position;
uniform sampler2D discoBallTex;

vec3 PointLightContribution(lightSource light) {
    //calculele componentelor difuze si speculare din modelul Phong de iluminare pentru lumina punctiforma

    float att = 0.0f;
    float light_radius = 1.2;

    vec3 light_dir = light.position - fragpos;

    vec3 R = reflect(-light_dir, normal);
    vec3 V = normalize(-(fragpos - eye));

    float dist = length(light_dir);
    if (dist < light_radius)
        att = pow(light_radius - dist, 1);

    float intensity = max(dot(light_dir, normal), 0);
    vec3 diffuse = att * intensity * light.color;

    vec3 specular = att * light.color * intensity * pow(max(dot(V, R), 0), 1);

    return diffuse + specular;
}
vec3 getDiscoBallLight() {
    vec3 color;
    vec3 light_dir = fragpos - disco_ball_position;

    // texcoord este coordonata de textura utilizata pentru esantionare
    vec2 texcoord;
    texcoord.x = (1.0 / (2 * 3.14159)) * atan(light_dir.x, light_dir.z) - time / 10;
    texcoord.y = (1.0 / 3.14159) * acos(light_dir.y / length(light_dir));

    // color este culoarea corespunzatoare pozitiei world_position
    color = texture (discoBallTex, texcoord).xyz;
    return color;
}

void main()
{
    vec3 color = vec3(0);

    if (type == Floor) {
       color = lights[tileIdx].color;
       color += 0.1f * getDiscoBallLight();
    } else if (type == Wall) {
        for (int i = 0; i < 64; i++) {
            color += PointLightContribution(lights[i]); // *0.06;
        }
        color += 0.7f * getDiscoBallLight();
        // color = vec3(0.3);
    } else if (type == DiscoBall) {
        //color = vec3(0, 0, 1);
        color += getDiscoBallLight();
    }
    else if (type == Dancer) {
        // TODO
        color = vec3(0, 1, 0);
    }

    out_color = vec4(color, 1);
}

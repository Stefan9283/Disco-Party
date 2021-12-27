#version 330

#define PI 3.141592653589793

#define Floor 1
#define DiscoBall 2
#define Dancer 3
#define Wall 4
#define Spotlight 5

// Output
layout(location = 0) out vec4 out_color;

// Input
in vec3 normal;
in vec3 fragpos;

// structs
struct lightSource {
    vec3 color;
    vec3 position;
};
struct directionalLightSource {
    vec3 direction;
    vec3 color;
    vec3 position;
    float cutoff;
};

// Uniforms
uniform vec3 eye;
uniform float time;

uniform int type;

uniform int idx; // only if type == Floor
uniform lightSource lights[100]; // the first 64 one are the tiles

uniform directionalLightSource spotlights[4];

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
    texcoord.x = (1.0 / (2 * PI)) * atan(light_dir.x, light_dir.z) - time / 10;
    texcoord.y = (1.0 / PI) * acos(light_dir.y / length(light_dir));

    // color este culoarea corespunzatoare pozitiei world_position
    color = texture(discoBallTex, texcoord).xyz;
    return color;
}
vec3 getSpotlightLight(directionalLightSource spotlight) {
    vec3 L = normalize(spotlight.position - fragpos);
    vec3 V = normalize(eye - fragpos);
    vec3 H = normalize(L + V);
    float d = distance(fragpos, spotlight.position);
    float attenuation_factor = 1 / pow(d, 2);
    float primesteLumina = 1;
    if (dot(L, V) < 0) primesteLumina = 0;
    vec3 ambient_light = spotlight.color * 0.3f;
    vec3 diffuse_light = spotlight.color * max(dot(normal, L), 0);
    int n = 10;
    vec3 R = reflect(-L, normal);
    vec3 specular_light;
    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Phong (1975) shading method. Don't mix them up!


    if (primesteLumina > 0) {
        specular_light = //material_ks * 
            vec3(1) * pow(max(dot(V, R), 0), n);
    }

    // TODO(student): If (and only if) the light is a spotlight, we need to do
    // some additional things.
    vec3 color;
    float spot_light = dot(-L, spotlight.direction);
    float spot_light_limit = cos(radians(spotlight.cutoff));
    float light_att_factor = 0;
    if (spot_light > spot_light_limit) {
        // Quadratic attenuation
        float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
        light_att_factor = pow(linear_att, 2);
    }
    color = (ambient_light + (diffuse_light + specular_light) * light_att_factor);
    return color;
}
void main() {
    vec4 color = vec4(0);
    if (type == Floor) {
       color = vec4(lights[idx].color, 1);
       color += 0.1f * vec4(getDiscoBallLight(), 1);
    } 
    else if (type == Wall) {
        for (int i = 0; i < 64; i++) {
            color += vec4(PointLightContribution(lights[i]), 1); // *0.06;
        }
        color += vec4(getDiscoBallLight(), 1);
        //color = vec4(0.3);
        color.w = 1;
    } 
    else if (type == DiscoBall) {
        //color = vec3(0, 0, 1);
        color += vec4(getDiscoBallLight(), 1);
    }
    else if (type == Dancer) {
        for (int i = 0; i < 64; i++) {
            color += vec4(PointLightContribution(lights[i]), 1);
        }
        color += 0.7f * vec4(getDiscoBallLight(), 1);
        //color = vec4(0, 1, 0);
    }
    else if (type == Spotlight) {
        // TODO
        color = vec4(spotlights[idx].color, 0.4);
    }

    out_color = color;
}

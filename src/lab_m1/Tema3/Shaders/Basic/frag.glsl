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
uniform int mode;

uniform vec3 eye;
uniform float time;

uniform int type;

uniform int idx; // only if type == Spotlight
uniform lightSource lights[9];

uniform directionalLightSource spotlights[4];

uniform vec3 disco_ball_position;
uniform sampler2D discoBallTex;

vec3 PointLightContribution(lightSource light) {
    //calculele componentelor difuze si speculare din modelul Phong de iluminare pentru lumina punctiforma
    float att = 0.0f;
    float light_radius = 1.2f;

    vec3 light_dir = light.position - fragpos;

    vec3 R = reflect(-light_dir, normal);
    vec3 V = normalize(-(fragpos - eye));

    float dist = length(light_dir);
    if (dist < light_radius)
        att = pow(light_radius - dist, 1);

    float intensity = max(dot(light_dir, normal), 0);

    vec3 diffuse =  light.color;
    vec3 specular = light.color * pow(max(dot(V, R), 0), 1);

    return att * intensity * (diffuse + specular);
}
vec3 getDiscoBallLight() {
    
    vec3 light_dir = fragpos - disco_ball_position;

    // texcoord este coordonata de textura utilizata pentru esantionare
    vec2 texcoord;
    texcoord.x = (1.0 / (2 * PI)) * atan(light_dir.x, light_dir.z) - time / 10;
    texcoord.y = (1.0 / PI) * acos(light_dir.y / length(light_dir));

    // color este culoarea corespunzatoare pozitiei world_position
    vec3 color = texture(discoBallTex, texcoord).xyz;

    lightSource discoball;
    discoball.color = vec3(1);
    discoball.position = disco_ball_position;

    return PointLightContribution(discoball) + color;
}
vec3 getSpotlightLight(directionalLightSource spotlight) {
    vec3 L = normalize(fragpos - spotlight.position);
    vec3 V = normalize(fragpos - eye);
    vec3 H = normalize(L + V);
    float d = distance(fragpos, spotlight.position);
    float attenuation_factor = 1 / pow(d, 2);
    float primesteLumina = 1;

    if (dot(L, V) < 0) primesteLumina = 0;

    vec3 ambient_light = spotlight.color * 0.13f;
    vec3 diffuse_light = spotlight.color* max(dot(normal, -L), 0);
    int n = 10;
    vec3 R = reflect(-L, normal);
    vec3 specular_light = vec3(0);
    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Phong (1975) shading method. Don't mix them up!


    if (primesteLumina > 0) {
        specular_light = vec3(0.1) * pow(max(dot(V, R), 0), n);
    }

    // If (and only if) the light is a spotlight, we need to do
    // some additional things.
    float spot_light = dot(-L, normalize(-spotlight.direction)); // tetha
    float spot_light_limit = cos(radians(spotlight.cutoff));
    float light_att_factor = 0;
    if (spot_light > spot_light_limit) {
        // Quadratic attenuation
        float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
        light_att_factor = pow(linear_att, 2);
    }

    vec3 color = (ambient_light + diffuse_light + specular_light) * light_att_factor;
    return color;
}

void main() {
    vec4 color = vec4(0);
    float discoBallFactor = 0.4f;
    if (type == Floor) {
       if (mode == 0 || mode == 1) color += vec4(lights[0].color, 1);
       if (mode == 0 || mode == 3) color += vec4(0.4f * getDiscoBallLight(), 1);
       if (mode == 0 || mode == 2)
           for (int i = 0; i <= 3; i++)
               color += vec4(getSpotlightLight(spotlights[i]), 1);
    } else if (type == Wall) {
        if (mode == 0 || mode == 1)
            for (int i = 0; i < 9; i++)
                color += vec4(PointLightContribution(lights[i]), 1);
        if (mode == 0 || mode == 3) color += vec4(discoBallFactor * getDiscoBallLight(), 1);
    } else if (type == DiscoBall) {
        if (mode == 0 || mode == 3) color += vec4(getDiscoBallLight(), 1);
    } else if (type == Dancer) {
        if (mode == 0 || mode == 1)
            for (int i = 0; i < 9; i++) {
                color += vec4(PointLightContribution(lights[i]), 1);
            }
        if (mode == 0 || mode == 2)
            for (int i = 0; i <= 3; i++)
                color += vec4(getSpotlightLight(spotlights[i]), 1);
        if (mode == 0 || mode == 3) color += 0.7f * vec4(discoBallFactor * getDiscoBallLight(), 1);
        color.w = 1;
    } else if (type == Spotlight) {
        if (mode == 0 || mode == 2) color += vec4(spotlights[idx].color, 0.4);
    }

    out_color = color;
}

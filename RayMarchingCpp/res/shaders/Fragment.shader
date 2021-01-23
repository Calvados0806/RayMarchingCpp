#version 330 core
#define MAX_STEPS 100
#define MAX_DISTANCE 50.0
#define SURFACE_DISTANCE 0.001

out vec4 color;

uniform vec2 u_Resolution;
uniform vec3 u_LightPos;

uniform vec3 u_CameraPos;
uniform float u_CameraRotY;
uniform int u_EnableShadows;
uniform float u_SmoothMinValue;

/*<uniforms>*/

float smin(float v1, float v2, float d)
{
    float h = clamp(0.5 + 0.5 * (v2 - v1) / d, 0.0, 1.0);
    return mix(v2, v1, h) - d * h * (1.0 - h);
}

mat2 Rotate(float a)
{
    float c = cos(a);
    float s = sin(a);
    return mat2(c, -s, s, c);
}

/*<dist_functions>*/

float GetSceneDistance(vec3 cameraPos)
{
    /*<scene_dist_code>*/
    return 0.0f;
}

vec3 GetNormal(vec3 pointPos)
{
    float dist = GetSceneDistance(pointPos);

    vec3 normal = dist - vec3(
        GetSceneDistance(pointPos - vec3(SURFACE_DISTANCE, 0, 0)),
        GetSceneDistance(pointPos - vec3(0, SURFACE_DISTANCE, 0)),
        GetSceneDistance(pointPos - vec3(0, 0, SURFACE_DISTANCE))
    );

    return normalize(normal);
}

float RayMarch(vec3 ro, vec3 rd, out vec3 pointPos)
{
    float totalDistance = 0.0;
    vec3 currCameraPos;

    for (int i = 0; i < MAX_STEPS; i++) {
        // One step in direction of ray
        currCameraPos = ro + rd * totalDistance;
        float sceneDistance = GetSceneDistance(currCameraPos);
        totalDistance += sceneDistance;

        if (totalDistance > MAX_DISTANCE) {
            break;
            //return 0.1;
        }

        if (sceneDistance < SURFACE_DISTANCE) {
            break;
        }
    }
    pointPos = currCameraPos;

    return totalDistance;
}

vec3 GetLight(vec3 pointPos)
{
    vec3 lightPos = u_LightPos;
    vec3 lightVec = lightPos - pointPos;
    vec3 lightDir = normalize(lightVec);
    vec3 pointNormal = GetNormal(pointPos);

    float diffuse = clamp(dot(pointNormal, lightDir) * 0.5 + 0.5, 0.0, 1.0);
    if (bool(u_EnableShadows)) {
        vec3 dummy;
        float lightDist = RayMarch(pointPos + pointNormal * SURFACE_DISTANCE, lightDir, dummy);

        if (lightDist < length(lightVec)) {
            diffuse *= 0.2;
        }
    }

    return vec3(diffuse);
}

// Will generate fog effect
vec3 GetDistanceDiffuse(float totalDistance)
{
    return vec3(clamp(1.0 - totalDistance / MAX_DISTANCE, 0, 1));
}

vec4 ColorizedRayMarch(vec3 ro, vec3 rd)
{
    vec3 pointPos;
    float distance = RayMarch(ro, rd, pointPos);

    vec3 color = GetLight(pointPos);
    // Apply fog effect
    color *= GetDistanceDiffuse(distance);
    return vec4(color, 1.0);
}

void main()
{
    vec2 uv = (gl_FragCoord.xy - 0.5 * u_Resolution) / u_Resolution.y;

    // Camera position
    vec3 ro = u_CameraPos;
    // Camera ray direction
    vec3 rd = vec3(uv.x, uv.y, 1.0);
    rd.xz *= Rotate(-u_CameraRotY);
    rd = normalize(rd);

    color = ColorizedRayMarch(ro, rd);
}
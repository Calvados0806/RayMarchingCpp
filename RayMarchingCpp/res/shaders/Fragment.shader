#version 330 core
#define MAX_STEPS 100
#define MAX_DISTANCE 100.0
#define SURFACE_DISTANCE 0.01

out vec4 color;

uniform vec2 u_Resolution;

float GetSceneDistance(vec3 cameraPos)
{
    // shere coordinates x, y, z and radius w
    vec4 sphere = vec4(0, 1, 6, 1);

    float sphereDist = length(cameraPos - sphere.xyz) - sphere.w;
    float planeDist = cameraPos.y;

    return min(sphereDist, planeDist);
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

        if (totalDistance > MAX_DISTANCE || sceneDistance < SURFACE_DISTANCE) {
            break;
        }
    }
    pointPos = currCameraPos;

    return totalDistance;
}

vec3 GetNormal(vec3 pointPos)
{
    float dist = GetSceneDistance(pointPos);

    vec3 normal = dist - vec3(
        GetSceneDistance(pointPos - vec3(0.01, 0, 0)),
        GetSceneDistance(pointPos - vec3(0, 0.01, 0)),
        GetSceneDistance(pointPos - vec3(0, 0, 0.01))
    );

    return normalize(normal);
}

float GetLight(vec3 pointPos)
{
    vec3 lightPos = vec3(0, 5, 6);
    lightPos.xz += vec2(sin(40), cos(40)) * 3.0;
    vec3 lightDir = normalize(lightPos - pointPos);
    vec3 pointNormal = GetNormal(pointPos);

    float diffuse = clamp(dot(pointNormal, lightDir), 0.0, 1.0);
    return diffuse;
}

void main()
{
    vec2 uv = (gl_FragCoord.xy - 0.5 * u_Resolution) / u_Resolution.y;

    // Camera position
    vec3 ro = vec3(0, 1, 0);
    // Camera ray direction
    vec3 rd = normalize(vec3(uv.x, uv.y, 1.0));

    vec3 pointPos;
    float pointDist = RayMarch(ro, rd, pointPos);

    float diffuse = GetLight(pointPos);

    color = vec4(vec3(diffuse), 1.0);
}
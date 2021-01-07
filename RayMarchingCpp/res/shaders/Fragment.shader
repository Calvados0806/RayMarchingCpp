#version 330 core
#define MAX_STEPS 100
#define MAX_DISTANCE 1000.0
#define SURFACE_DISTANCE 0.001

out vec4 color;

uniform vec2 u_Resolution;
// sphere coordinates x, y, z and radius w
uniform vec4 u_SphereObj;
uniform float u_PlaneObj;
uniform vec3 u_LightPos;

const int kObjects = 2;
float gDistances[kObjects];

float GetMin()
{
    float res = gDistances[0];
    for (int i = 1; i < kObjects; i++) {
        res = min(res, gDistances[i]);
    }
    return res;
}

float SphereDist(vec3 cameraPos)
{
    return length(cameraPos - u_SphereObj.xyz) - u_SphereObj.w;
}

float PlaneDist(vec3 cameraPos)
{
    return abs(cameraPos.y - u_PlaneObj);
}

float GetSceneDistance(vec3 cameraPos)
{
    gDistances[0] = SphereDist(cameraPos);
    gDistances[1] = PlaneDist(cameraPos);

    return GetMin();
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
        GetSceneDistance(pointPos - vec3(SURFACE_DISTANCE, 0, 0)),
        GetSceneDistance(pointPos - vec3(0, SURFACE_DISTANCE, 0)),
        GetSceneDistance(pointPos - vec3(0, 0, SURFACE_DISTANCE))
    );

    return normalize(normal);
}

float GetLight(vec3 pointPos)
{
    vec3 lightPos = u_LightPos;
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
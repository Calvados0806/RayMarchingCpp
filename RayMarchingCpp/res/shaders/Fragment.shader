#version 330 core
#define MAX_STEPS 100
#define MAX_DISTANCE 50.0
#define SURFACE_DISTANCE 0.001

out vec4 color;

uniform vec2 u_Resolution;
// sphere coordinates x, y, z and radius w
uniform vec4 u_SphereObj;
uniform float u_PlaneObj;
uniform vec3 u_LightPos;
uniform vec4 u_CubeObj;

uniform vec3 u_CameraPos;
uniform float u_CameraRotY;

const int kObjects = 3;
float gDistances[kObjects];

mat2 Rotate(float a)
{
    float c = cos(a);
    float s = sin(a);
    return mat2(c, -s, s, c);
}

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

float CubeDist(vec3 cameraPos)
{
    vec3 size = vec3(u_CubeObj.w);
    vec3 p = cameraPos - u_CubeObj.xyz;
    vec3 d = abs(p) - size;
    return min(max(d.x, max(d.y, d.z)), 0.0) +
        length(max(d, 0.0));
}

float GetSceneDistance(vec3 cameraPos)
{
    gDistances[0] = SphereDist(cameraPos);
    gDistances[1] = PlaneDist(cameraPos);
    gDistances[2] = CubeDist(cameraPos);

    return GetMin();
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
    vec3 dummy;
    float lightDist = RayMarch(pointPos + pointNormal * SURFACE_DISTANCE, lightDir, dummy);

    if (lightDist < length(lightVec)) {
        diffuse *= 0.2;
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
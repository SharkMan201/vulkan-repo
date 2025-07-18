#version 330 core

layout (triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec2 texCoords;
    vec3 normal;
    vec3 fragPos;
} gs_in[];

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;
uniform float time;

vec3 GetNormal() {
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);

    return normalize(cross(a, b));
}

vec4 explode(vec4 position, vec3 normal) {
    float magnitude = 2.0;
    vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude;
//    return position + vec4(direction, 0.0);
    return position;
}

void main() {
    // I think I could've gotten the normal immediately as input from vs
    // OOOOR, if I don't have normals maybe I can have geometry shader calculate it for me
    // INTERESTING STUFF
    vec3 normal = GetNormal();

    gl_Position = explode(gl_in[0].gl_Position, normal);
    TexCoords = gs_in[0].texCoords;
    Normal = gs_in[0].normal;
    FragPos = gs_in[0].fragPos;
    EmitVertex();

    gl_Position = explode(gl_in[1].gl_Position, normal);
    TexCoords = gs_in[1].texCoords;
    Normal = gs_in[1].normal;
    FragPos = gs_in[1].fragPos;
    EmitVertex();

    gl_Position = explode(gl_in[2].gl_Position, normal);
    TexCoords = gs_in[2].texCoords;
    Normal = gs_in[2].normal;
    FragPos = gs_in[2].fragPos;
    EmitVertex();

    EndPrimitive();
}
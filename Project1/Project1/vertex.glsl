

layout (location = 0) in vec3 Position;


out vec2 TexCoords0;

void main()
{
    gl_Position =vec4(Position, 1.0);
}
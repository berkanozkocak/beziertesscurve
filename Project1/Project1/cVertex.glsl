#version 400

layout (location = 0) in vec2 Position;
uniform mat4 gWVP;
out vec2 center;

void main()
{
    gl_Position =gWVP * vec4(Position.xy,0.0, 1.0);
    center =  gl_Position.xy;
}
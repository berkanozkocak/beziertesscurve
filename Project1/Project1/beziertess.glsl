
layout (isolines) in;

uniform mat4 gWVP;

void main()
{
    float t = gl_TessCoord.x;

    vec3 p0 = gl_in[0].gl_Position.xyz;
    vec3 p1 = gl_in[1].gl_Position.xyz;
    vec3 p2 = gl_in[2].gl_Position.xyz;
    vec3 p3 = gl_in[3].gl_Position.xyz;

    float t1 = (1.0 - t);
    float t2 = t * t;

    // Bernstein polynomials
    float b3 = t2 * t;
    float b2 = 3.0 * t2 * t1;
    float b1 = 3.0 * t * t1 * t1;
    float b0 = t1 * t1 * t1;

    // Cubic Bezier interpolation
    vec3 p = p0 * b0 + p1 * b1 + p2 * b2 + p3 * b3;

    gl_Position = gWVP * vec4(p, 1.0);
}   
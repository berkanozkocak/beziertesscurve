#version 400
    out vec4 outColor;
    uniform vec3 mouseP;
    in vec2 center;
    void main() {
        float dis =  distance(center.xy,mouseP.xy);
        if(dis > 0.10f){
            outColor = vec4(1.0, 0.0, 0.0, 1.0);
            return;
        }

        outColor = vec4(0.0, 1.0, 0.0, 1.0);
    }
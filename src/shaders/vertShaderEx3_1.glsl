#version 430

uniform mat4 rotationMatrix; // Transformationsmatrix als uniform

void main(void) {
    vec4 vertices[3] = vec4[3](
        vec4(0.25, -0.25, 0.0, 1.0),
        vec4(-0.25, -0.25, 0.0, 1.0),
        vec4(0.0, 0.25, 0.0, 1.0)
    );

    gl_Position = rotationMatrix * vertices[gl_VertexID]; 
}

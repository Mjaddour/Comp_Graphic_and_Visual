#ifndef VERTEXSHADER_SOURCELIGHT_H
#define VERTEXSHADER_SOURCELIGHT_H


// Shader source code for vertex and fragment shaders for cube light
const char* vertexShaderSourceLight = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    void main()
    {
        gl_Position = projection * view * model * vec4(aPos, 1.0);
    }
)";

#endif
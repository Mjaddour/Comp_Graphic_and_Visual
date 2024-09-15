#ifndef FRAGMENTSHADER_SOURCELIGHT_H
#define FRAGMENTSHADER_SOURCELIGHT_H


const char* fragmentShaderSourceLight = R"(
    #version 330 core
    out vec4 FragColor;

    void main()
    {
        FragColor = vec4(1.0f); // White light
    }
)";
#endif
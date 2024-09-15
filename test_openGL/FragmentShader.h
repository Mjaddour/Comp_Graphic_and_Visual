#ifndef FRAGMENT_SHADER_H
#define FRAGMENT_SHADER_H

const char* fragmentShaderSource = R"(
    #version 330 core
    in vec2 TexCoord; // Input texture coordinate from vertex shader
    out vec4 FragColor;

    uniform sampler2D texture1; // Texture uniform

    void main()
    {
        FragColor = texture(texture1, TexCoord); // Sample texture at the given texture coordinate
    }
)";

#endif 
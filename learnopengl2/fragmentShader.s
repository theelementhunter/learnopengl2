#version 410 core
    out vec4 FragColor;
    in vec4 Pos;
    void main()
    {
       FragColor = Pos;
    }
#version 410 core
    layout (location = 0) in vec3 aPos;
    uniform  float horizontalOffset;
    void main()
    {
       gl_Position = vec4(aPos.x + horizontalOffset, aPos.y, aPos.z, 1.0);
    }
#version 330 core
in vec3 TexDir;
out vec4 FragColor;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
uniform sampler2D tex4;
uniform sampler2D tex5;

void main()
{
    vec3 d = normalize(TexDir);

    if (abs(d.x) > abs(d.y) && abs(d.x) > abs(d.z))
        FragColor = (d.x > 0) ? texture(tex0, d.yz) : texture(tex1, d.yz);
    else if (abs(d.y) > abs(d.x) && abs(d.y) > abs(d.z))
        FragColor = (d.y > 0) ? texture(tex2, d.xz) : texture(tex3, d.xz);
    else
        FragColor = (d.z > 0) ? texture(tex4, d.xy) : texture(tex5, d.xy);
}
#version 140

in vec2 textureCoords;
in vec4 colourPass;

in vec2 thicknessPass;

out vec4 out_Color;

uniform sampler2D tex;

void main(void)
{
    vec4 colour = texture(tex, vec2(textureCoords.x, 1 - textureCoords.y));
    float distance = 1 - colour.a;
    float step = 1 - smoothstep(thicknessPass.x, thicknessPass.x + thicknessPass.y, distance);
    //if(step * colourPass.a > 0.1)
    {
	    out_Color = vec4(colourPass.xyz, step * colourPass.a);
    }

}
#version 450 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D sceneTexture;
uniform sampler2D depthTexture;

uniform float focusDistance;
uniform float focusRange;
uniform float maxBlur;

float LinearizeDepth(float depth)
{
    float near = 0.1; // Distance du plan proche (doit correspondre à votre configuration)
    float far  = 1000.0; // Distance du plan lointain (doit correspondre à votre configuration)
    float z = depth * 2.0 - 1.0; // De [0,1] à [-1,1]
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
    float depth = texture(depthTexture, TexCoords).r;
    float linearDepth = LinearizeDepth(depth);

    float coc = abs(linearDepth - focusDistance) / focusRange;
    coc = clamp(coc, 0.0, 1.0);

    float blurRadius = coc * maxBlur;

    vec3 color = vec3(0.0);
    int samples = 10;
    int halfSamples = samples / 2;
    for (int x = -halfSamples; x <= halfSamples; x++)
    {
        for (int y = -halfSamples; y <= halfSamples; y++)
        {
            vec2 offset = vec2(float(x), float(y)) * blurRadius / float(samples);
            color += texture(sceneTexture, TexCoords + offset).rgb;
        }
    }
    color /= float(samples * samples);

    FragColor = vec4(color, 1.0);
}

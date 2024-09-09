#version 450 core
out vec4 FragColor;

uniform vec3 lightColor;
uniform bool visualizeDepth;

float near = 0.1f;
float far = 90.0f;

float LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0; // Back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
    if (visualizeDepth) FragColor = vec4(vec3(LinearizeDepth(gl_FragCoord.z) / far), 1.0);
    else FragColor = vec4(lightColor, 1.0);
}
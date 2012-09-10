uniform vec3 lightDir;
varying vec3 normal;
void main()
{
    float intensity = dot(lightDir, normal);
    gl_FragColor =  0.5 * (1.5 + intensity) * gl_Color;
}

varying vec3 normal;
 
void main()
{   
 
    // the following three lines provide the same result
 
//  gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
//  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    gl_Position = ftransform();
    gl_FrontColor = gl_Color;
    gl_BackColor = gl_Color;
    normal =  gl_Normal;
    normal = gl_NormalMatrix  * normal;
}

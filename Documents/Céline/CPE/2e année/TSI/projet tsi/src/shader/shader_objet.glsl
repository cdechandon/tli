#version 330

#ifdef VERTEX_SHADER

layout(location= 0) in vec3 position;
layout(location= 1) in vec2 texcoord;
layout(location= 2) in vec3 norm;
layout(location= 3) in vec4 couleur;

uniform mat4 matrix;
uniform mat4 mod_o;
uniform vec3 soleil;
uniform float time;
mat4 rot=mat4(cos(-time/50), 0, sin(-time/50), 0, 0, 1, 0, 0, -sin(-time/50), 0, cos(-time/50), 0, 0, 0, 0, 1);

out vec3 color;
out vec2 vertex_texcoord;

void main( )
{
    vertex_texcoord=texcoord;
    color=vec3(0,0,0);
    gl_Position= matrix * vec4(position, 1);
    vec3 N=norm ; // normale du triangle
    vec4 soleil_place=mod_o*vec4(soleil,1);
    vec3 L= vec3(soleil_place.xyz)-position ; // direction du soleil
    //vec3 L=vec3(100,200,100)-position;
    float cos_theta= dot(normalize(N), normalize(L));
    //color= color*cos_theta *1;
}
#endif

#ifdef FRAGMENT_SHADER
in vec3 color;
in vec2 vertex_texcoord;
uniform sampler2D texture0;

void main( )
{
    //gl_FragColor=texelFetch(texture0, ivec2(vertex_texcoord),10)*color;
    //gl_FragColor=texture(texture0,vertex_texcoord)*color;
    gl_FragColor=color;
}
#endif

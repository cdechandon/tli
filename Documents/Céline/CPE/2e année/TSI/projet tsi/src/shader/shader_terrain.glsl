#version 330
// source: Arthur Lemaout
#ifdef VERTEX_SHADER
layout(location= 0) in vec3 position;
layout(location= 1) in vec2 texcoord;
layout(location= 2) in vec3 normal;

uniform mat4 matrice_mvp;
uniform mat4 matrice_mv;

out vec3 vertex_position;
out vec2 vertex_texcoord;
out vec3 vertex_normal;
//out vec3 color;
void main( )
{
    gl_Position= matrice_mvp * vec4(position, 1);
   // color=position.yyy/10;
    vertex_position= vec3(matrice_mv * vec4(position, 1));
    vertex_texcoord= texcoord;
    vertex_normal= mat3(matrice_mv) * normal;
}

#endif


#ifdef FRAGMENT_SHADER

uniform vec4 couleur_diffuse;
uniform sampler2D diffuse_texture;

in vec3 vertex_position;
in vec2 vertex_texcoord;
in vec3 vertex_normal;
//in vec3 color;
out vec4 fragment_color;

void main( )
{
    float cos_theta= abs(dot(normalize(- vertex_position), normalize(vertex_normal)));

    //utilise la texture
    vec4 color_texture= texture(diffuse_texture, vertex_texcoord);
    if(color_texture.a < 0.3)
      discard;
    
    // module la couleur de la matiere par la couleur de la texture
    vec3 color= couleur_diffuse.rgb * color_texture.rgb * cos_theta;
    
    // applique une correction gamma au resultat
    fragment_color= vec4(color, 1);
}

#endif

#version 330

#ifdef VERTEX_SHADER
layout(location= 0) in vec3 position;
layout(location= 3) in vec4 couleur;
uniform mat4 matrix;

out vec4 color;
// shader permettant d'afficher le repere en couleur
void main( )
{
    if (position.x>0){
        color=vec4(1,0,0,0);
    }
    else if (position.y>0){
        color=vec4(0,1,0,0);
    }
    else if (position.z>0){
        color=vec4(0,0,1,0);
    }
    else{
        color=vec4(1,1,1,0);
    }
    //color=couleur;
    gl_Position=matrix*vec4(position,1);
}
#endif

#ifdef FRAGMENT_SHADER
in vec4 color;
void main( )
{
    gl_FragColor=color;
}
#endif

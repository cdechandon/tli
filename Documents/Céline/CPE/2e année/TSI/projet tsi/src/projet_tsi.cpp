// Celine Dechandon et Seraphin Andrieux 4ETI
// Nous voulions faire une copie d'un labyrinthe
// Merci au groupe de Arthur Lemaout et Bouvard Clara de nous avoir aidé pour les shaders et l'affichage de nos objets
// source labyrinthe : https://www.yobi3d.com/q/labyrinthe?ext=obj&page=1





// Declaration des bibliotheques
#include <stdio.h>
#include "window.h"
#include "mesh.h"
#include "wavefront.h"
#include "draw.h"
#include "color.h"
#include "image.h"
#include "image_io.h"
#include "program.h"
#include "mat.h"
#include "material_data.h"
#include "mesh_buffer.h"
#include "mesh_data.h"
#include "uniforms.h"


//Realisation d'une camera a partir de orbiter.h
struct Cameras{ //représente une caméra
    Point centre_obs;
    vec2 position_relative;
    vec2 rotation_relative;
    float distance_obs;
    float rayon_boite_obv=1000;
};

// Variables globales
Cameras camera; //Declaration d'une camera qui s'appellera camera

GLuint prog_repere,vect_repere, buffer_position_repere; //declaration pour le repere
GLuint prog_cube,vao_cube, vbo_cube, ivbo_cube, sampler_cube; //declaration pour notre vehicule (pour l'instant un cube)
GLuint prog_terrain,vao_terrain, vbo_terrain, ivbo_terrain, sampler_terrain; //declaration pour le terrain (circuit luigi)

Mesh repere;


MeshData data_terrain;
MeshBuffer terrain;

MeshData data_cube;
MeshBuffer cube;

Point pminv,pmaxv;
Point pmint,pmaxt;

//Declaration des fonctions permettant a notre camera de fonctionner (en se basant sur orbiter.h
void obs_pt_dist(Cameras *camera, const Point centre, const float distance){ //modifie les parametres de la caméra pour observer le point centre par rapport à la distance donnee en paramètre
    camera->centre_obs=centre;
    camera->position_relative=vec2(0,0);
    camera->rotation_relative=vec2(0,0);
    camera->distance_obs=distance;
}

void obs_pts(Cameras *camera, const Point min, const Point max){ //modifie les parametres de la caméra pour observer les points en paramètre
    obs_pt_dist(camera,center(min,max),distance(min,max));
}

Transform vue(Cameras *camera){ //renvoie la transformation 
    return Translation(-camera->position_relative.x, -camera->position_relative.y, -camera->distance_obs )
           * RotationX(camera->rotation_relative.x) * RotationY(camera->rotation_relative.y)
           * Translation( - Vector(camera->centre_obs) );
}

Transform projection(Cameras *camera, const float width, const float height, const float fov){ 
    // calcule la distance entre le centre de l'objet et la camera
    float d= distance(camera->centre_obs, Point(camera->position_relative.x, camera->position_relative.y, camera->distance_obs));
    return Perspective(fov, width / height, std::max(0.1f, d - camera->rayon_boite_obv), std::max(1.f, d + camera->rayon_boite_obv));
}

void translation_cam(Cameras *camera, const float x, const float y){ //fonction permettant de deplacer la camera
    camera->position_relative.x=camera->position_relative.x-camera->distance_obs*x;
    camera->position_relative.y=camera->position_relative.y+camera->distance_obs*y;
}

void rotation_cam(Cameras *camera, const float x, const float y){ //fonction permettant de faire tourner la camera
    camera->rotation_relative.x+=y;
    camera->rotation_relative.y+=x;
}

void zoom_cam(Cameras *camera, const float z){ //fonction permettant d'effectuer un zoom
    camera->distance_obs= camera->distance_obs - camera->distance_obs * 0.01f * z;
    if(camera->distance_obs < 0.01f){
        camera->distance_obs= 0.01f;
    }
}

void affich_param_cam(){ 
    printf("Camera:\nCentre:(%f,%f,%f)\nPos rel: (%f,%f)\nRot rel: (%f,%f)\nDist: %f\nRayon: %f\n\n",camera.centre_obs.x,
           camera.centre_obs.y,camera.centre_obs.z,camera.position_relative.x,camera.position_relative.y,camera.rotation_relative.x,
           camera.rotation_relative.y,camera.distance_obs,camera.rayon_boite_obv);
}

//Initialisation des objets

void init_repere(int taille_repere){ //initialise un repère en fonction d'une taille donnee
    repere=Mesh(GL_LINES); 
    repere.vertex(0,0,0);
    repere.vertex(taille_repere,0,0);
    repere.vertex(0,0,0);
    repere.vertex(0,taille_repere,0);
    repere.vertex(0,0,0);
    repere.vertex(0,0,taille_repere);

    prog_repere= read_program("./src/shader/shader_repere.glsl");
    program_print_errors(prog_repere);// affiche les erreurs

    //création puis sélection du vertex array du repère
    glGenVertexArrays(1, &vect_repere);
    glBindVertexArray(vect_repere);

    //création, sélection puis initialisation du buffer position du repère
    const std::vector<vec3> pos_rep=repere.positions();
    glGenBuffers(1,&buffer_position_repere);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_position_repere);
    glBufferData(GL_ARRAY_BUFFER,pos_rep.size()*sizeof(vec3),(const float*) pos_rep.data(), GL_STATIC_DRAW);

    GLuint attrib_position_repere=glGetAttribLocation(prog_repere, "position");
    glVertexAttribPointer(attrib_position_repere,3,GL_FLOAT,GL_FALSE,sizeof(vec3),0);
    glEnableVertexAttribArray(attrib_position_repere);
}




int init_vehicule(){ // initialisation de notre vehicule (ici un cube)
    prog_cube=read_program("./src/shader/shader_terrain.glsl");
    program_print_errors(prog_cube);

    data_cube= read_mesh_data("data/cube.obj");
    //preparer les buffers pour l'affichage

    normals(data_cube); //recalcule les normales

    cube= buffers(data_cube);

    // vérifie que le maillage contient au moins un sommet..
    if (cube.positions.size() == 0)// test s'il y a une erreur
        return -1;

    // création et sélection du vertex du vehicule
    glGenVertexArrays(1, &vao_cube);
    glBindVertexArray(vao_cube);

    // construction du vertex buffer 
    size_t taille= cube.vertex_buffer_size() + cube.texcoord_buffer_size() +cube.normal_buffer_size();
    glGenBuffers(1, &vbo_cube);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube);
    glBufferData(GL_ARRAY_BUFFER, taille, nullptr, GL_STATIC_DRAW);

    // transfere les positions des sommets
    size_t decalage= 0; // position dans le buffer à partir de laquelle seront stockées les positions des sommets
    taille= cube.vertex_buffer_size();
    glBufferSubData(GL_ARRAY_BUFFER, decalage, taille, cube.vertex_buffer());
    // et configure l'attribut 0, vec3 position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, /* stride */ 0, (const GLvoid *) decalage);
    glEnableVertexAttribArray(0);

    // transfere les texcoords des sommets
    decalage= decalage + taille; // position dans le buffer à partir de laquelle seront stockées les texcoords des sommets
    taille= cube.texcoord_buffer_size();
    glBufferSubData(GL_ARRAY_BUFFER, decalage, taille, cube.texcoord_buffer());
    // et configure l'attribut 1, vec2 texcoord
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, /* stride */ 0, (const GLvoid *) decalage);
    glEnableVertexAttribArray(1);

    // transfere les normales des sommets
    decalage= decalage + taille; // position dans le buffer à partir de laquelle seront stockées les normales des sommets
    taille= cube.normal_buffer_size();
    glBufferSubData(GL_ARRAY_BUFFER, decalage, taille, cube.normal_buffer());
    // et configure l'attribut 2, vec3 normal
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, /* stride */ 0, (const GLvoid *) decalage);
    glEnableVertexAttribArray(2);

    // index buffer, contient les indices des triangles, triés par groupes de matières par la fonction buffers
    glGenBuffers(1, &ivbo_cube);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ivbo_cube);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube.index_buffer_size(), cube.index_buffer(), GL_STATIC_DRAW);

    // nettoyage
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // charge les textures des matieres
    read_textures(cube.materials);

    // configure le filtrage des textures de l'unite 0
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8.0f);

    // configure le filtrage des textures, mode repeat
    glGenSamplers(1, &sampler_cube);
    glSamplerParameteri(sampler_cube, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glSamplerParameteri(sampler_cube, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(sampler_cube, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glSamplerParameteri(sampler_cube, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //Limites du vehicule
    bounds(data_cube,pminv,pmaxv);


    return 0;
}

int init_terrain(){
    prog_terrain=read_program("./src/shader/shader_terrain.glsl");
    program_print_errors(prog_terrain);

    //data_terrain= read_mesh_data("data/test2/fg laby.obj.obj");
	data_terrain= read_mesh_data("data/test2/fg laby.obj.obj");
    //preparer les buffers pour l'affichage
    terrain= buffers(data_terrain);

    // vérifie que le maillage contient au moins un sommet..
    if (terrain.positions.size() == 0)
        // erreur, rien à afficher...
        return -1;

    // création et sélection du vertex array terrain
    glGenVertexArrays(1, &vao_terrain);
    glBindVertexArray(vao_terrain);

    // construction du vertex buffer pour les trois attributs
    size_t taille= terrain.vertex_buffer_size() + terrain.texcoord_buffer_size() +terrain.normal_buffer_size();
    glGenBuffers(1, &vbo_terrain);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_terrain);
    glBufferData(GL_ARRAY_BUFFER, taille, nullptr, GL_STATIC_DRAW);

    // transfere les positions des sommets
    size_t decalage= 0; // position dans le buffer à partir de laquelle seront stockées les positions des sommets
    taille= terrain.vertex_buffer_size();
    glBufferSubData(GL_ARRAY_BUFFER, decalage, taille, terrain.vertex_buffer());
    // et configure l'attribut 0, vec3 position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, /* stride */ 0, (const GLvoid *) decalage);
    glEnableVertexAttribArray(0);

    // transfere les texcoords des sommets
    decalage= decalage + taille; // position dans le buffer à partir de laquelle seront stockées les texcoords des sommets
    taille= terrain.texcoord_buffer_size();
    glBufferSubData(GL_ARRAY_BUFFER, decalage, taille, terrain.texcoord_buffer());
    // et configure l'attribut 1, vec2 texcoord
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, /* stride */ 0, (const GLvoid *) decalage);
    glEnableVertexAttribArray(1);

    // transfere les normales des sommets
    decalage= decalage + taille; // position dans le buffer à partir de laquelle seront stockées les normales des sommets
    taille= terrain.normal_buffer_size();
    glBufferSubData(GL_ARRAY_BUFFER, decalage, taille, terrain.normal_buffer());
    // et configure l'attribut 2, vec3 normal
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, /* stride */ 0, (const GLvoid *) decalage);
    glEnableVertexAttribArray(2);

    // index buffer, contient les indices des triangles, triés par groupes de matières par la fonction buffers
    glGenBuffers(1, &ivbo_terrain);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ivbo_terrain);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, terrain.index_buffer_size(), terrain.index_buffer(), GL_STATIC_DRAW);

    // nettoyage
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // charge les textures des matieres
    read_textures(terrain.materials);

    // configure le filtrage des textures de l'unite 0
    glActiveTexture(GL_TEXTURE0); // selectionne l'unite de texture 0
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8.0f);

    // configure le filtrage des textures, mode repeat
    glGenSamplers(1, &sampler_terrain);
    glSamplerParameteri(sampler_terrain, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glSamplerParameteri(sampler_terrain, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(sampler_terrain, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glSamplerParameteri(sampler_terrain, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //Limites du terrain
    bounds(data_terrain,pmint,pmaxt);

    printf("_____\nnb: %lud\n_____\n",terrain.material_indices.size());

    return 0;
}

//Fonctions de dessin des objet

void draw_vehicule(Transform mvp_vehicule, Transform mv_vehicule){
    glUseProgram(prog_cube);
    GLint l_vehicule_mvp= glGetUniformLocation(prog_cube, "matrice_mvp");
    glUniformMatrix4fv(l_vehicule_mvp, 1, GL_TRUE, mvp_vehicule.buffer());
    GLint l_vehicule_mv= glGetUniformLocation(prog_cube, "matrice_mv");
    glUniformMatrix4fv(l_vehicule_mv, 1, GL_TRUE, mv_vehicule.buffer());
    vec4 couleur_diffuse= vec4(1, 1, 0, 1);
    GLint l_vehicule_dc= glGetUniformLocation(prog_cube, "couleur_diffuse");
    glUniform4fv(l_vehicule_dc, 1, &couleur_diffuse.x);

    glBindVertexArray(vao_cube);
    for(int i= 0; i < (int)cube.material_groups.size(); i++){// boucle permettant d'afficher notre vehicule
        const MaterialData& material= cube.materials[cube.material_groups[i].material];

        glUniform4fv(l_vehicule_dc, 1, &material.diffuse.r); 

        glActiveTexture(GL_TEXTURE0); 
        glBindTexture(GL_TEXTURE_2D, material.diffuse_texture);

        GLint location_vehicule= glGetUniformLocation(prog_cube, "diffuse_texture");
        glUniform1i(location_vehicule, 0);

        glBindSampler(0, sampler_cube);
        glDrawElements(GL_TRIANGLES, cube.material_groups[i].count,
            GL_UNSIGNED_INT, cube.index_buffer_offset(cube.material_groups[i].first));
    }
}

void draw_terrain(Transform mvp_ter,Transform mv_ter){
    glUseProgram(prog_terrain);
    GLint l_ter_mvp= glGetUniformLocation(prog_terrain, "matrice_mvp");
    glUniformMatrix4fv(l_ter_mvp, 1, GL_TRUE, mvp_ter.buffer());
    GLint l_ter_mv= glGetUniformLocation(prog_terrain, "matrice_mv");
    glUniformMatrix4fv(l_ter_mv, 1, GL_TRUE, mv_ter.buffer());
    vec4 couleur_diffuse= vec4(1, 1, 0, 1);
    GLint l_ter_dc= glGetUniformLocation(prog_terrain, "couleur_diffuse");
    glUniform4fv(l_ter_dc, 1, &couleur_diffuse.x);

    glBindVertexArray(vao_terrain);
    for(int i= 0; i < (int)terrain.material_groups.size(); i++){//boucle permettant d'afficher notre terrain
        const MaterialData& material= terrain.materials[terrain.material_groups[i].material];

        glUniform4fv(l_ter_dc, 1, &material.diffuse.r);

        glActiveTexture(GL_TEXTURE0); 
        glBindTexture(GL_TEXTURE_2D, material.diffuse_texture); 
        GLint location= glGetUniformLocation(prog_terrain, "diffuse_texture");
        glUniform1i(location, 0);
        glBindSampler(0, sampler_terrain); 
        glDrawElements(GL_TRIANGLES, terrain.material_groups[i].count,
            GL_UNSIGNED_INT, terrain.index_buffer_offset(terrain.material_groups[i].first));

    }
}



float alt_t(const float xc, const float yc, MeshBuffer& mesh, Transform mod,int j,bool affich){
    std::vector<vec3> positions= mesh.positions;
    Point ref=(Point)(positions[0]);
    for(int i=0;i<(int)positions.size();i++){
        Point pt=mod((Point)positions[i]);

        if (distance(Point(xc,0,yc),Point(pt.x,0,pt.y))<distance(Point(xc,0,yc),Point(ref.x,0,ref.y))){
              ref=pt;
          }
        else if (sqrt((xc-pt.x)*(xc-pt.x)+(yc-pt.y)*(yc-pt.y))==sqrt((xc-ref.x)*(xc-ref.x)+(yc-ref.y)*(yc-ref.y))){

            if (pt.z>ref.z){
                ref=pt;
            }
        }
    }
    if((j%60==0)&&affich==true){printf("ref: (%f,%f,%f)\n",ref.x,ref.y,ref.z);}
    return ref.z;
}


int init(){ //fonction d'initialisation du contexte et des objets

    int taille_repere=50; //taille des axes du repère
    init_repere(taille_repere);


    if (init_vehicule()<0){return -1;}

    if (init_terrain()<0){return -1;}

    obs_pts(&camera,pmint,pmaxt);

    glClearColor(1, 1, 1, 1);


    glClearDepthf(1.f);	
    glDepthFunc(GL_LESS); 
    glEnable(GL_DEPTH_TEST); 

    return 0; //si pas d'erreur
}

int compteur=0; //compteur de frame, utilisé pour l'affichage
float alt_vehicule=0;
vec3 vt_vehicule=vec3(-0.1,0,alt_vehicule);
float angleY_vehicule=270;
int phase=0;
float vit_vehicule=0.1;



int draw(){ //fonction qui boucle en continue, 60 fois par secondes, une fois pour chaque image

    compteur+=1;
    //effacement de la fenetre avant de dessiner quelquechose
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //transformations afin de passer du repère objet au repère projectif
    Transform model_rep=Identity(); 

    vt_vehicule.z=alt_vehicule;
    if (key_state('s')){
        vt_vehicule.y-=0.1;
    }
    if (key_state('z')){
        vt_vehicule.y+=0.1;
    }
   if (key_state('q')){
        vt_vehicule.x+=0.1;
    }
   if (key_state('w')){
        vt_vehicule.x-=0.1;
    }
    Transform model_vehicule=Translation((Vector&)vt_vehicule)*Scale(0.8,0.8,0.8)*RotationY(angleY_vehicule);
    Point pos_vehicule=model_vehicule(center(pminv,pmaxv));


   switch (phase) { // TO DO permettant de gerer les collisions avec les murs
    case 0:
 
        break;
    case 1:

        break;
	case 2:


		break;
	case 3:

	
	break;
	case 4:

	break;
    default:

        break;

    }



    Transform model_ter=RotationX(270);

    float calc=alt_t(center(model_vehicule(pminv),model_vehicule(pmaxv)).x,center(model_vehicule(pminv),model_vehicule(pmaxv)).y,terrain,model_ter,compteur,false);
    if(abs(calc-alt_vehicule)<0.2||compteur==1){
        alt_vehicule=calc;
    }

    Transform view=vue(&camera); 

    Transform proj=projection(&camera, window_width(), window_height(), 45);

    Transform mvp_rep=proj*view*model_rep; //composition des transformation dans cet ordre car on compose à gauche et on veut d'abord appliquer model puis view puis projection

    Transform mv_vehicule=view*model_vehicule;
    Transform mvp_vehicule=proj*mv_vehicule;

    Transform mv_ter=view*model_ter;
    Transform mvp_ter=proj*mv_ter;


    //dessin du repère
    glUseProgram(prog_repere); //sélection du shader
    GLint l_rep_mvp= glGetUniformLocation(prog_repere, "matrice_mvp"); //récupération de la position de l'uniform correspondant à la matrice projection*view*model dans le shader
    glUniformMatrix4fv(l_rep_mvp, 1, GL_TRUE, mvp_rep.buffer()); //attribution de la valeur de t à l'uniform matrice_mvp
    glBindVertexArray(vect_repere); //sélection du vertex array du repère
    glDrawArrays(GL_LINES,0,repere.vertex_count()); //dessin du repère en lignes

    draw_vehicule(mvp_vehicule,mv_vehicule);
    draw_terrain(mvp_ter,mv_ter);



    //Pour deplacer la camera
    if (key_state(SDLK_SPACE)==1){
        obs_pt_dist(&camera,center(model_vehicule(pminv),model_vehicule(pmaxv)),2*distance(model_vehicule(pminv),model_vehicule(pmaxv)));
        rotation_cam(&camera,270,9);
    }
    if (key_state(SDLK_UP)==1){
        translation_cam(&camera,0,0.01);
    }
    if (key_state(SDLK_DOWN)==1){
        translation_cam(&camera,0,-0.01);
    }
    if (key_state(SDLK_LEFT)==1){
        translation_cam(&camera,0.01,0);
    }
    if (key_state(SDLK_RIGHT)==1){
        translation_cam(&camera,-0.01,0);
    }
    if (key_state('p')){
        zoom_cam(&camera,1);
    }
    if (key_state('m')){
        zoom_cam(&camera,-1);
    }
    int mx, my;
        unsigned int mb= SDL_GetRelativeMouseState(&mx, &my);

    if(mb & SDL_BUTTON(1))              // le bouton gauche est enfonce
            rotation_cam(&camera,mx, my);
    else if(mb & SDL_BUTTON(3))         // le bouton droit est enfonce
            zoom_cam(&camera,mx);
    if(key_state('r'))
        {
            clear_key_state('r');
            reload_program(prog_repere, "./src/shader/sh_rep.glsl");
            program_print_errors(prog_repere);
            reload_program(prog_cube, "./src/shader/sh_terrain.glsl");
            program_print_errors(prog_cube);
            reload_program(prog_terrain, "./src/shader/sh_terrain.glsl");
            program_print_errors(prog_terrain);
        }

    if (compteur%60==0){

        printf("Position : (%f,%f,%f)\n\n",pos_vehicule.x,pos_vehicule.y,pos_vehicule.z);
    }



    return 1; //tant qu'on veut continuer à boucler
}


int quit(){ //fonction qui libère les espaces mémoires utilisés
    repere.release();

    release_textures(cube.materials);
    glDeleteBuffers(1, &vbo_cube);
    glDeleteBuffers(1, &ivbo_cube);
    glDeleteVertexArrays(1, &vao_cube);

    release_textures(terrain.materials);
    glDeleteBuffers(1, &vbo_terrain);
    glDeleteBuffers(1, &ivbo_terrain);
    glDeleteVertexArrays(1, &vao_terrain);

    release_program(prog_cube);
    release_program(prog_repere);
    release_program(prog_terrain);

    return 0; //si pas d'erreur
}

int main(int argc, char **argv){
    // Création de la fenêtre
    Window window= create_window(1024, 640);
    if(window == NULL)
        return 1;       // erreur lors de la creation de la fenetre ou de l'init de sdl2

    //Création du contexte
    Context context= create_context(window);
    if(context == NULL)
        return 1;       // erreur lors de la creation du contexte opengl

    //Création des objets
    if(init() < 0)
    {
        printf("[error] init failed.\n");
        return 1;
    }

    // Affichage de l'application
    run(window, draw);

    // Nettoyage
    quit();
    release_context(context);
    release_window(window);
    return 0;
}

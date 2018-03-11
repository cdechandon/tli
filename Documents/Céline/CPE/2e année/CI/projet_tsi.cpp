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


//!Structures
struct Cameras{ //représente une caméra
    Point centre_obs;
    vec2 position_relative;
    vec2 rotation_relative;
    float distance_obs;
    float rayon_boite_obv=1000;
};

//!Variables globales
Cameras camera; //instance de la strucure Cameras qui représentera notre caméra

GLuint prog_rep,va_rep, buf_pos_rep; //shader program, vertex array et buffer position du repère
GLuint prog_kartM,vao_kartM, vbo_kartM, ivbo_kartM, sampler_kartM; //shader program, VAO, VBO, index buffer et sampler kart Mario
GLuint prog_ter,vao_ter, vbo_ter, ivbo_ter, sampler_ter; //shader program, VAO, VBO, index buffer et sampler terrain

Mesh repere; //maillage/objet repere


MeshData data_ter;
MeshBuffer terrain;

MeshData data_kartM;
MeshBuffer kartM;

Point pminkM,pmaxkM;
Point pmint,pmaxt;

//!Fonctions liées à la caméra
void obs_pt_dist(Cameras *camera, const Point centre, const float distance){ //modifie les attributs de la caméra pour observé le point centre à la distance en paramètre
    camera->centre_obs=centre;
    camera->position_relative=vec2(0,0);
    camera->rotation_relative=vec2(0,0);
    camera->distance_obs=distance;
}

void obs_pts(Cameras *camera, const Point min, const Point max){ //modifie les attributs de la caméra pour observer les points en paramètre
    obs_pt_dist(camera,center(min,max),distance(min,max));
}

Transform vue(Cameras *camera){ //renvoie la transformation qui place l'objet observé dans le repère caméra
    return Translation(-camera->position_relative.x, -camera->position_relative.y, -camera->distance_obs )
           * RotationX(camera->rotation_relative.x) * RotationY(camera->rotation_relative.y)
           * Translation( - Vector(camera->centre_obs) );
}

Transform projection(Cameras *camera, const float width, const float height, const float fov){ //renvoie la transformation qui place l'objet dans le repère projectif de la scène
    // calcule la distance entre le centre de l'objet et la camera
    float d= distance(camera->centre_obs, Point(camera->position_relative.x, camera->position_relative.y, camera->distance_obs));
    // regle near (coordonnée z du plan de vue xy le plus proche) et far (coordonnée z du plan de vue xy le plus proche) en fonction du centre et du rayon englobant l'objet
    return Perspective(fov, width / height, std::max(0.1f, d - camera->rayon_boite_obv), std::max(1.f, d + camera->rayon_boite_obv));
}

void translation_cam(Cameras *camera, const float x, const float y){ //déplace la caméra en translation selon le plan de l'écran
    camera->position_relative.x=camera->position_relative.x-camera->distance_obs*x;
    camera->position_relative.y=camera->position_relative.y+camera->distance_obs*y;
}

void rotation_cam(Cameras *camera, const float x, const float y){ //déplace la caméra en rotation selon les axes du plan de l'écran
    camera->rotation_relative.x+=y;
    camera->rotation_relative.y+=x;
}

void zoom_cam(Cameras *camera, const float z){ //approche ou éloigne la caméra de la scène
    camera->distance_obs= camera->distance_obs - camera->distance_obs * 0.01f * z;
    if(camera->distance_obs < 0.01f){
        camera->distance_obs= 0.01f;
    }
}

void affich_param_cam(){ //affiche les paramètres de la caméra
    printf("Camera:\nCentre:(%f,%f,%f)\nPos rel: (%f,%f)\nRot rel: (%f,%f)\nDist: %f\nRayon: %f\n\n",camera.centre_obs.x,
           camera.centre_obs.y,camera.centre_obs.z,camera.position_relative.x,camera.position_relative.y,camera.rotation_relative.x,
           camera.rotation_relative.y,camera.distance_obs,camera.rayon_boite_obv);
}

//!Fonctions d'initialisation des objets

void init_repere(int taille_rep){ //initialise un repère dont la taille des axes est donnée en paramètre
    repere=Mesh(GL_LINES); //utilisation de lignes et non de triangles comme primitives pour tracer le repère
    //création des sommets du repère. Par simplicité, les dessins sont ordonnés (et non indexés): on doit recréer un sommet à chaque nouvelle utilisation dans une primitive
    //axe des x
    repere.vertex(0,0,0);
    repere.vertex(taille_rep,0,0);
    //axe des y
    repere.vertex(0,0,0);
    repere.vertex(0,taille_rep,0);
    //axe des z
    repere.vertex(0,0,0);
    repere.vertex(0,0,taille_rep);

    //compilation du shader program du repère et affichage des erreurs
    prog_rep= read_program("./src/shader/sh_rep.glsl");
    program_print_errors(prog_rep);

    //création puis sélection du vertex array du repère
    glGenVertexArrays(1, &va_rep);
    glBindVertexArray(va_rep);

    //création, sélection puis initialisation du buffer position du repère
    const std::vector<vec3> pos_rep=repere.positions();
    glGenBuffers(1,&buf_pos_rep);
    glBindBuffer(GL_ARRAY_BUFFER, buf_pos_rep);
    glBufferData(GL_ARRAY_BUFFER,pos_rep.size()*sizeof(vec3),(const float*) pos_rep.data(), GL_STATIC_DRAW);

    //création puis activation de l'attribut position du repère (va et buffers sélectionnés implicitement ci-dessus
    GLuint attrib_pos_rep=glGetAttribLocation(prog_rep, "position");
    glVertexAttribPointer(attrib_pos_rep,3,GL_FLOAT,GL_FALSE,sizeof(vec3),0);
    glEnableVertexAttribArray(attrib_pos_rep);
}

int init_kartM(){
    prog_kartM=read_program("./src/shader/sh_terrain.glsl");
    program_print_errors(prog_kartM);

    data_kartM= read_mesh_data("data/shooting_star/kart_MR_b.obj");
    //preparer les buffers pour l'affichage

    normals(data_kartM); //recalcule les normales

    kartM= buffers(data_kartM);

    // vérifie que le maillage contient au moins un sommet..
    if (kartM.positions.size() == 0)
        // erreur, rien à afficher...
        return -1;

    // création et sélection du vertex array kartM
    glGenVertexArrays(1, &vao_kartM);
    glBindVertexArray(vao_kartM);

    // construction du vertex buffer pour les trois attributs
    size_t taille= kartM.vertex_buffer_size() + kartM.texcoord_buffer_size() +kartM.normal_buffer_size();
    glGenBuffers(1, &vbo_kartM);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_kartM);
    glBufferData(GL_ARRAY_BUFFER, taille, nullptr, GL_STATIC_DRAW);

    // transfere les positions des sommets
    size_t decalage= 0; // position dans le buffer à partir de laquelle seront stockées les positions des sommets
    taille= kartM.vertex_buffer_size();
    glBufferSubData(GL_ARRAY_BUFFER, decalage, taille, kartM.vertex_buffer());
    // et configure l'attribut 0, vec3 position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, /* stride */ 0, (const GLvoid *) decalage);
    glEnableVertexAttribArray(0);

    // transfere les texcoords des sommets
    decalage= decalage + taille; // position dans le buffer à partir de laquelle seront stockées les texcoords des sommets
    taille= kartM.texcoord_buffer_size();
    glBufferSubData(GL_ARRAY_BUFFER, decalage, taille, kartM.texcoord_buffer());
    // et configure l'attribut 1, vec2 texcoord
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, /* stride */ 0, (const GLvoid *) decalage);
    glEnableVertexAttribArray(1);

    // transfere les normales des sommets
    decalage= decalage + taille; // position dans le buffer à partir de laquelle seront stockées les normales des sommets
    taille= kartM.normal_buffer_size();
    glBufferSubData(GL_ARRAY_BUFFER, decalage, taille, kartM.normal_buffer());
    // et configure l'attribut 2, vec3 normal
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, /* stride */ 0, (const GLvoid *) decalage);
    glEnableVertexAttribArray(2);

    // index buffer, contient les indices des triangles, triés par groupes de matières par la fonction buffers
    glGenBuffers(1, &ivbo_kartM);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ivbo_kartM);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, kartM.index_buffer_size(), kartM.index_buffer(), GL_STATIC_DRAW);

    // nettoyage
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // charge les textures des matieres
    read_textures(kartM.materials);

    // configure le filtrage des textures de l'unite 0
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8.0f);

    // configure le filtrage des textures, mode repeat
    glGenSamplers(1, &sampler_kartM);
    glSamplerParameteri(sampler_kartM, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glSamplerParameteri(sampler_kartM, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(sampler_kartM, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glSamplerParameteri(sampler_kartM, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //Limites du kart
    bounds(data_kartM,pminkM,pmaxkM);

    return 0;
}

int init_terrain(){
    prog_ter=read_program("./src/shader/sh_terrain.glsl");
    program_print_errors(prog_ter);

    data_ter= read_mesh_data("data/luigi_circuit/Luigi Circuit.obj");
    //preparer les buffers pour l'affichage
    terrain= buffers(data_ter);

    // vérifie que le maillage contient au moins un sommet..
    if (terrain.positions.size() == 0)
        // erreur, rien à afficher...
        return -1;

    // création et sélection du vertex array terrain
    glGenVertexArrays(1, &vao_ter);
    glBindVertexArray(vao_ter);

    // construction du vertex buffer pour les trois attributs
    size_t taille= terrain.vertex_buffer_size() + terrain.texcoord_buffer_size() +terrain.normal_buffer_size();
    glGenBuffers(1, &vbo_ter);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_ter);
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
    glGenBuffers(1, &ivbo_ter);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ivbo_ter);
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
    glGenSamplers(1, &sampler_ter);
    glSamplerParameteri(sampler_ter, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glSamplerParameteri(sampler_ter, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(sampler_ter, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glSamplerParameteri(sampler_ter, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //Limites du terrain
    bounds(data_ter,pmint,pmaxt);

    printf("_____\nnb: %lud\n_____\n",terrain.material_indices.size());

    return 0;
}

//!Fonctions de dessin des objet

void draw_kartM(Transform mvp_kartM, Transform mv_kartM){
    glUseProgram(prog_kartM);
    GLint l_kartM_mvp= glGetUniformLocation(prog_kartM, "matrice_mvp");
    glUniformMatrix4fv(l_kartM_mvp, 1, GL_TRUE, mvp_kartM.buffer());
    GLint l_kartM_mv= glGetUniformLocation(prog_kartM, "matrice_mv");
    glUniformMatrix4fv(l_kartM_mv, 1, GL_TRUE, mv_kartM.buffer());
    vec4 couleur_diffuse= vec4(1, 1, 0, 1);
    GLint l_kartM_dc= glGetUniformLocation(prog_kartM, "couleur_diffuse");
    glUniform4fv(l_kartM_dc, 1, &couleur_diffuse.x);

    glBindVertexArray(vao_kartM);
    for(int i= 0; i < (int)kartM.material_groups.size(); i++){
        const MaterialData& material= kartM.materials[kartM.material_groups[i].material];

        glUniform4fv(l_kartM_dc, 1, &material.diffuse.r); //couleur diffuse de la matière

        // utilise une texture
        glActiveTexture(GL_TEXTURE0); // selectionne l'unite de texture 0
        glBindTexture(GL_TEXTURE_2D, material.diffuse_texture); // selectionne la texture

         // parametre le shader avec le numero de l'unite sur laquelle est selectionee la texture
        GLint location_kM= glGetUniformLocation(prog_kartM, "diffuse_texture");
        glUniform1i(location_kM, 0);

        glBindSampler(0, sampler_kartM); // sélectionne le sampler (parametres de filtrage)

        // afficher chaque sequence de triangles
        glDrawElements(GL_TRIANGLES, kartM.material_groups[i].count,
            GL_UNSIGNED_INT, kartM.index_buffer_offset(kartM.material_groups[i].first));
    }
}

void draw_terrain(Transform mvp_ter,Transform mv_ter){
    glUseProgram(prog_ter);
    GLint l_ter_mvp= glGetUniformLocation(prog_ter, "matrice_mvp");
    glUniformMatrix4fv(l_ter_mvp, 1, GL_TRUE, mvp_ter.buffer());
    GLint l_ter_mv= glGetUniformLocation(prog_ter, "matrice_mv");
    glUniformMatrix4fv(l_ter_mv, 1, GL_TRUE, mv_ter.buffer());
    vec4 couleur_diffuse= vec4(1, 1, 0, 1);
    GLint l_ter_dc= glGetUniformLocation(prog_ter, "couleur_diffuse");
    glUniform4fv(l_ter_dc, 1, &couleur_diffuse.x);

    glBindVertexArray(vao_ter);
    for(int i= 0; i < (int)terrain.material_groups.size(); i++){
        const MaterialData& material= terrain.materials[terrain.material_groups[i].material];

        glUniform4fv(l_ter_dc, 1, &material.diffuse.r); //couleur diffuse de la matière

        // utilise une texture
        glActiveTexture(GL_TEXTURE0); // selectionne l'unite de texture 0
        glBindTexture(GL_TEXTURE_2D, material.diffuse_texture); // selectionne la texture

         // parametre le shader avec le numero de l'unite sur laquelle est selectionee la texture
        GLint location= glGetUniformLocation(prog_ter, "diffuse_texture");
        glUniform1i(location, 0);

        glBindSampler(0, sampler_ter); // sélectionne le sampler (parametres de filtrage)

        // afficher chaque sequence de triangles
        glDrawElements(GL_TRIANGLES, terrain.material_groups[i].count,
            GL_UNSIGNED_INT, terrain.index_buffer_offset(terrain.material_groups[i].first));

    }
}


//!Autres fonctions

float alt_t(const float xc, const float zc, MeshBuffer& mesh, Transform mod,int j,bool affich){
    std::vector<vec3> positions= mesh.positions;
    Point ref=(Point)(positions[0]);
    for(int i=0;i<(int)positions.size();i++){
        Point pt=mod((Point)positions[i]);

        if (distance(Point(xc,0,zc),Point(pt.x,0,pt.z))<distance(Point(xc,0,zc),Point(ref.x,0,ref.z))){
 //         if (sqrt((xc-pt.x)*(xc-pt.x)+(zc-pt.z)*(zc-pt.z))<sqrt((xc-ref.x)*(xc-ref.x)+(zc-ref.z)*(zc-ref.z))){
              ref=pt;
          }
        else if (sqrt((xc-pt.x)*(xc-pt.x)+(zc-pt.z)*(zc-pt.z))==sqrt((xc-ref.x)*(xc-ref.x)+(zc-ref.z)*(zc-ref.z))){

            if (pt.y>ref.y){
                ref=pt;
            }
        }
    }
    if((j%60==0)&&affich==true){printf("ref: (%f,%f,%f)\n",ref.x,ref.y,ref.z);}
    return ref.y;
}

//!Les trois fonctions essentielles à tout dessin opengl
int init(){ //fonction d'initialisation du contexte et des objets

    int taille_rep=50; //taille des axes du repère
    init_repere(taille_rep);


    if (init_kartM()<0){return -1;}

    if (init_terrain()<0){return -1;}

    //poscam
    obs_pts(&camera,pmint,pmaxt);


    //changement de la couleur par défaut en bleu ciel pour donner un effet ciel au fond
    glClearColor(0.686f, 0.886f, 0.949f, 1);

    //conservation du triangle le plus proche uniquement en cas de conflit
    glClearDepthf(1.f);	// profondeur par defaut
    glDepthFunc(GL_LESS);  // conserver le triangle le plus proche,
    glEnable(GL_DEPTH_TEST); // activation du test

    return 0; //si pas d'erreur
}

int cpt=0; //compteur de frame, utilisé pour l'affichage
float alt_kM=0;
vec3 vt_kM=vec3(27,alt_kM,19.3);
float angleY_kM=270;
int phase=0;
float vit_kM=0.1;

int draw(){ //fonction qui boucle en continue, 60 fois par secondes, une fois pour chaque image

    cpt+=1;
    //effacement de la fenetre avant de dessiner quelquechose
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //transformations afin de passer du repère objet au repère projectif
    Transform model_rep=Identity(); //passage du repère objet au repère monde: on place l'objet dans le monde

    vt_kM.y=alt_kM;
    if (key_state('z')){
        vt_kM.x-=0.1;
    }
    if (key_state('s')){
        vt_kM.x+=0.1;
    }
    Transform model_kartM=Translation((Vector&)vt_kM)*RotationY(angleY_kM);
    Point pos_kM=model_kartM(center(pminkM,pmaxkM));


    switch (phase) {
    case 0:
        vt_kM.x-=vit_kM;
        if (pos_kM.x<8.867){
            phase=1;
        }
        break;
    case 1:

        break;
    default:
        break;
    }



    Transform model_ter=RotationX(270);

    float calc=alt_t(center(model_kartM(pminkM),model_kartM(pmaxkM)).x,center(model_kartM(pminkM),model_kartM(pmaxkM)).z,terrain,model_ter,cpt,false);
    if(abs(calc-alt_kM)<0.2||cpt==1){
        alt_kM=calc;
    }

    Transform view=vue(&camera); //passage du repère monde au repère caméra

    Transform proj=projection(&camera, window_width(), window_height(), 45); //passage du repère caméra au repère de projection 2D + z pour savoir quel objet est le plus proche

    Transform mvp_rep=proj*view*model_rep; //composition des transformation dans cet ordre car on compose à gauche et on veut d'abord appliquer model puis view puis projection

    Transform mv_kartM=view*model_kartM;
    Transform mvp_kartM=proj*mv_kartM;

    Transform mv_ter=view*model_ter;
    Transform mvp_ter=proj*mv_ter;

    //position du soleil
    //vec3 soleil=vec3(0,1000,0);

    //dessin du repère
    glUseProgram(prog_rep); //sélection du shader
    GLint l_rep_mvp= glGetUniformLocation(prog_rep, "matrice_mvp"); //récupération de la position de l'uniform correspondant à la matrice projection*view*model dans le shader
    glUniformMatrix4fv(l_rep_mvp, 1, GL_TRUE, mvp_rep.buffer()); //attribution de la valeur de t à l'uniform matrice_mvp
    glBindVertexArray(va_rep); //sélection du vertex array du repère
    glDrawArrays(GL_LINES,0,repere.vertex_count()); //dessin du repère en lignes

    draw_kartM(mvp_kartM,mv_kartM);
    draw_terrain(mvp_ter,mv_ter);



    //mouvements de camera
    if (key_state(SDLK_SPACE)==1){
        obs_pt_dist(&camera,center(model_kartM(pminkM),model_kartM(pmaxkM)),2.5*distance(model_kartM(pminkM),model_kartM(pmaxkM)));
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
            reload_program(prog_rep, "./src/shader/sh_rep.glsl");
            program_print_errors(prog_rep);
            reload_program(prog_kartM, "./src/shader/sh_terrain.glsl");
            program_print_errors(prog_kartM);
            reload_program(prog_ter, "./src/shader/sh_terrain.glsl");
            program_print_errors(prog_ter);
        }

    //affichage deux fois par seconde
    if (cpt%60==0){
       // affich_param_cam();

        printf("Position kart : (%f,%f,%f)\n\n",pos_kM.x,pos_kM.y,pos_kM.z);
    }



    return 1; //tant qu'on veut continuer à boucler
}


int quit(){ //fonction qui libère les espaces mémoires utilisés
    repere.release();

    release_textures(kartM.materials);
    glDeleteBuffers(1, &vbo_kartM);
    glDeleteBuffers(1, &ivbo_kartM);
    glDeleteVertexArrays(1, &vao_kartM);

    release_textures(terrain.materials);
    glDeleteBuffers(1, &vbo_ter);
    glDeleteBuffers(1, &ivbo_ter);
    glDeleteVertexArrays(1, &vao_ter);

    release_program(prog_kartM);
    release_program(prog_rep);
    release_program(prog_ter);

    return 0; //si pas d'erreur
}

int main(int argc, char **argv){
    // 1: Création de la fenêtre
    Window window= create_window(1024, 640);
    if(window == NULL)
        return 1;       // erreur lors de la creation de la fenetre ou de l'init de sdl2

    // 2: Création du contexte opengl
    Context context= create_context(window);
    if(context == NULL)
        return 1;       // erreur lors de la creation du contexte opengl

    // 3: Création des objets
    if(init() < 0)
    {
        printf("[error] init failed.\n");
        return 1;
    }

    // 3: Affichage de l'application, tant que la fenetre n'est pas fermée ou que draw() ne renvoie pas 0
    run(window, draw);

    // 5: Nettoyage
    quit();
    release_context(context);
    release_window(window);
    return 0;
}

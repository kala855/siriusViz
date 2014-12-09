#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GL/glew.h>
#include <GL/glut.h>

#include "./common/shader_utils.h"

#include "./utilities/utilidades.h"

GLuint program;
GLint attribute_coord2d;
GLint uniform_offset_x;
GLint uniform_scale_x;
GLint uniform_sprite;
GLuint texture_id;
GLint uniform_mytexture;

float offset_x = 0.0;
float scale_x = 1.0;
int mode = 1;

struct point {
  GLfloat x;
  GLfloat y;
};

GLuint vbo;


int init_resources(char *fileName) {

    int numData;
    float *x,*y,*vx,*vy;
    FILE *initFile;
    initFile = fopen(fileName,"r");
    if(initFile==NULL){
        printf("Archivo inexistente, verifique\n");
        return (0);
    }

    printf("Use left/right to move horizontally.\n");
    printf("Use up/down to change the horizontal scale.\n");
    printf("Press home to reset the position and scale.\n");
    printf("Press F1 to draw lines.\n");
    printf("Press F2 to draw points.\n");
    fscanf(initFile,"%d", &numData);

    // allocate arrays on host
    size_t size = numData*sizeof(float);

    x = (float *)malloc(size);
    y = (float *)malloc(size);
    vy = (float *)malloc(size);
    vx = (float *)malloc(size);

    for (int i = 0; i < numData; ++i) {
        fscanf(initFile,"%f %f", &x[i],&vx[i]);//&vx[i],&vy[i]);
    }

    fclose(initFile);

    program = create_program("graph.v.glsl", "graph.f.glsl");
    if (program == 0)
        return 0;

    attribute_coord2d = get_attrib(program, "coord2d");
    uniform_offset_x = get_uniform(program, "offset_x");
    uniform_scale_x = get_uniform(program, "scale_x");
    uniform_sprite = get_uniform(program, "sprite");
    uniform_mytexture = get_uniform(program, "mytexture");

    if (attribute_coord2d == -1 || uniform_offset_x == -1 || uniform_scale_x == -1 || uniform_sprite == -1 || uniform_mytexture == -1)
        return 0;

    /* Enable blending */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Enable point sprites (not necessary for true OpenGL ES 2.0) */
#ifndef GL_ES_VERSION_2_0
    glEnable(GL_POINT_SPRITE);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
#endif

    // Create the vertex buffer object
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Create our own temporary buffer
    point graph[10000];

    float minimoX, maximoX, minimoVX, maximoVX, maxAbs;
    maximoX = max(x,numData);
    minimoX = min(x,numData);
    maximoVX = max(vx,numData);
    minimoVX = min(vx,numData);
    maxAbs = max2Numbers(maximoVX, minimoVX);
    //  printf("El valor mínimo del vector x es : %f", minimo);
    // printf("El valor máximo del vector x es : %f", maximo);

    // Fill it in just like an array
    for (int i = 0; i < numData+1; i++) {
        //float x = (i - 1000.0) / 100.0;
        //Debido a que la pantalla cuando se usa openGL se divide en un plano
        //cartesianos que va desde -1 a 1 tanto en x como en y. Se hace
        //necesario realizarle una transformación a los datos para que estos salgan
        //centrados.
        //En este caso y debido a que las partículas para las que se inicia
        //la construcción de esta librería oscilan entre 0.0 - 25.0 en x y entre
        //-7.1568 - 7.0319 para y la transformación es la siguiente
        //La idea para la transformación es la siguiente
        //dataTransformado = (data/(max(data)/2.0)) - 1; Para datos > a cero
        //dataTransformado = (data/max(max(data),min(data))); Para datos > y < a cero
        graph[i].x = (x[i]/(maximoX/2.0)) - 1.0;//x;
        graph[i].y = (vx[i])/(maxAbs);//sin(x * 10.0) / (1.0 + x * x);
    }

    free(x); free(y); free(vx); free(vy);

    // Tell OpenGL to copy our array to the buffer object
    glBufferData(GL_ARRAY_BUFFER, sizeof graph, graph, GL_DYNAMIC_DRAW);

    return 1;
}

void animation(int file) {

    int numData;
    float *x,*y,*vx,*vy;
    FILE *initFile;
    char fileName[3];

    sprintf(fileName,"%d",file);

    initFile = fopen(fileName,"r");
    if(initFile==NULL){
        printf("Archivo inexistente %s animation, verifique\n",fileName);
        exit (1);
    }

   fscanf(initFile,"%d", &numData);

    // allocate arrays on host
    size_t size = numData*sizeof(float);

    x = (float *)malloc(size);
    y = (float *)malloc(size);
    vy = (float *)malloc(size);
    vx = (float *)malloc(size);

    for (int i = 0; i < numData; ++i) {
        fscanf(initFile,"%f %f", &x[i],&vx[i]);//&vx[i],&vy[i]);
    }

    fclose(initFile);
    point graph[10000];

    float minimoX, maximoX, minimoVX, maximoVX, maxAbs;
    maximoX = max(x,numData);
    minimoX = min(x,numData);
    maximoVX = max(vx,numData);
    minimoVX = min(vx,numData);
    maxAbs = max2Numbers(maximoVX, minimoVX);
    //  printf("El valor mínimo del vector x es : %f", minimo);
    // printf("El valor máximo del vector x es : %f", maximo);

    // Fill it in just like an array
    for (int i = 0; i < numData+1; i++) {
        //float x = (i - 1000.0) / 100.0;
        //Debido a que la pantalla cuando se usa openGL se divide en un plano
        //cartesianos que va desde -1 a 1 tanto en x como en y. Se hace
        //necesario realizarle una transformación a los datos para que estos salgan
        //centrados.
        //En este caso y debido a que las partículas para las que se inicia
        //la construcción de esta librería oscilan entre 0.0 - 25.0 en x y entre
        //-7.1568 - 7.0319 para y la transformación es la siguiente
        //La idea para la transformación es la siguiente
        //dataTransformado = (data/(max(data)/2.0)) - 1; Para datos > a cero
        //dataTransformado = (data/max(max(data),min(data))); Para datos > y < a cero
        graph[i].x = (x[i]/(maximoX/2.0)) - 1.0;//x;
        graph[i].y = (vx[i])/(maxAbs);//sin(x * 10.0) / (1.0 + x * x);
    }

    free(x); free(y); free(vx); free(vy);

    // Tell OpenGL to copy our array to the buffer object
    glBufferData(GL_ARRAY_BUFFER, sizeof graph, graph, GL_STATIC_DRAW);

    glutPostRedisplay();
    file++;
    if (file<11)
      glutTimerFunc(500,animation,file);
}


void display() {
    glUseProgram(program);
    glUniform1i(uniform_mytexture, 0);

    glUniform1f(uniform_offset_x, offset_x);
    glUniform1f(uniform_scale_x, scale_x);

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    /* Draw using the vertices in our vertex buffer object */
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glEnableVertexAttribArray(attribute_coord2d);
    glVertexAttribPointer(attribute_coord2d, 2, GL_FLOAT, GL_FALSE, 0, 0);

    /* Push each element in buffer_vertices to the vertex shader */
    switch (mode) {
    case 0:
      glUniform1f(uniform_sprite, 0);
      glDrawArrays(GL_LINE_STRIP, 0, 10000);
      break;
    case 1:
      glUniform1f(uniform_sprite, 1);
      glDrawArrays(GL_POINTS, 0, 10000);
      break;
  }
  glutSwapBuffers();
}

void special(int key, int x, int y) {
	switch (key) {
    case GLUT_KEY_F1:
      mode = 0;
      printf("Now drawing using lines.\n");
      break;
    case GLUT_KEY_F2:
      mode = 1;
      printf("Now drawing using points.\n");
      break;
    case GLUT_KEY_LEFT:
      offset_x -= 0.1;
      break;
    case GLUT_KEY_RIGHT:
      offset_x += 0.1;
      break;
    case GLUT_KEY_UP:
      scale_x *= 1.5;
      break;
    case GLUT_KEY_DOWN:
        scale_x /= 1.5;
        break;
    case GLUT_KEY_HOME:
        offset_x = 0.0;
        scale_x = 1.0;
        break;
    }
    glutPostRedisplay();
}

void free_resources() {
	glDeleteProgram(program);
}

int main(int argc, char *argv[]) {

  printf("Cantidad de Argumentos: %d\n", argc);

  if(argc < 2){
    printf("Para usar la aplicación se debe indicar un archivo de entrada\n");
    exit(1);
  }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Library Test");

    GLenum glew_status = glewInit();

    if (GLEW_OK != glew_status) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
        return 1;
    }

    if (!GLEW_VERSION_2_0) {
        fprintf(stderr, "No support for OpenGL 2.0 found\n");
        return 1;
    }

    if (init_resources(argv[1])) {
        glutDisplayFunc(display);
        glutTimerFunc(1500, animation,0);
        glutSpecialFunc(special);
        glutMainLoop();
    }

    free_resources();
    return 0;
}

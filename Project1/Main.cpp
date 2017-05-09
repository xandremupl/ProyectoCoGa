#include <windows.h>	//Inclusion de variables de windows

#include <glut.h>	//Inclusion de GLUT
#include <GL/gl.h>		//Inclusion de GL		 	
#include <GL/glu.h>	
#include <vector>
#include <stdio.h>
#include <math.h>	//Inclusion de librerias auxiliares	
#include "Camara_teclado.h"
const int W_WIDTH = 500;	 //Ancho de la ventana
const int W_HEIGHT = 500;		//Alto de la ventana

int cuadrado;
int cubo;
int esfera;


#define GL_PI 3.14f


// Ángulos de rotación
//static GLfloat xRot = 0.0f;
//static GLfloat yRot = 0.0f;

//Variables externas
extern float rotarX;
extern float rotarY;
extern float rotarZ;
extern float moverX;
extern float moverY;
extern float moverZ;

//Funciones externas
extern int myCuadrado();
extern int myCubo();
extern int myEsfera();

enum TIPO_MENU {
	MENU_PRINCIPAL,
	MENU_OBJETOS,
	MENU_TEXTURAS,
	MENU_ILUMINACION,
	CUBO,
	USER1,
	USER2,
	CEMENTO,
	CRISTAL,
	HIERBA,
	LUZ1,
	LUZ2,
	LUZ3,
};

typedef struct {
	float px, py, pz; // posicion
	float rx, ry, rz; // angulos de giro
	float sx, sy, sz; // escalado en los tres ejes.
	int listaRender; // lista de render
} Objeto;

std::vector<Objeto> objetos;
Objeto aux;

Objeto inicializarObjeto(float px, float py, float pz, float rx, float ry, float rz, float sx,
	float sy, float sz, int listaRender) {
	Objeto objeto;
	objeto.px = px;
	objeto.py = py;
	objeto.pz = pz;
	objeto.rx = rx;
	objeto.ry = ry;
	objeto.rz = rz;
	objeto.sx = sx;
	objeto.sy = sy;
	objeto.sz = sz;
	objeto.listaRender = listaRender;
	return(objeto);
}

void debuxarObjeto(Objeto objeto) {
	glTranslatef(objeto.px, objeto.py, objeto.pz);
	glRotatef(objeto.rx, 1.0f, 0.0f, 0.0f);
	glRotatef(objeto.ry, 0.0f, 1.0f, 0.0f);
	glRotatef(objeto.rz, 0.0f, 0.0f, 1.0f);
	glScalef(objeto.sx, objeto.sy, objeto.sz);
	glCallList(objeto.listaRender);
}

GLboolean comprobarColision(Objeto obj1, Objeto obj2) {
	bool colisionX = (obj1.px + obj1.sx > obj2.px) && (obj2.px + obj2.sx > obj1.px);
	bool colisionY = (obj1.py + obj1.sy > obj2.py) && (obj2.py + obj2.sy > obj1.py);
	bool colisionZ = (obj1.pz + obj1.sz > obj2.pz) && (obj2.pz + obj2.sz > obj1.pz);

	return(colisionX && colisionY && colisionZ);
}

void modColExtern() {
	moverX = objetos[objetos.size() - 1].px;
	moverY = objetos[objetos.size() - 1].py;
	moverZ = objetos[objetos.size() - 1].pz;
	rotarX = objetos[objetos.size() - 1].rx;
	rotarY = objetos[objetos.size() - 1].ry;
	rotarZ = objetos[objetos.size() - 1].rz;
}

void modColObjeto() {
	objetos[objetos.size() - 1].px = moverX;
	objetos[objetos.size() - 1].py = moverY;
	objetos[objetos.size() - 1].pz = moverZ;
	objetos[objetos.size() - 1].rx = rotarX;
	objetos[objetos.size() - 1].ry = rotarY;
	objetos[objetos.size() - 1].rz = rotarZ;
}

// Funcion de dibukop
void myDisplay(void) {
	int i;
	GLboolean colision=FALSE;


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the window with current clearing color

	myCamara();

	// Incluye una matriz de rotacion en todos los puntos que se dibujen
	glMatrixMode(GL_MODELVIEW);
	// Inicializamos la matriz del modelo a la identidad
	glLoadIdentity();
	if (objetos.size() > 0) {
		for (i = 0; i < objetos.size() - 1; i++) {
			glPushMatrix();

			glColor3f(1.0f, 1.0f, 1.0f);
			debuxarObjeto(objetos[i]);

			glPopMatrix();
		}

		glPushMatrix();

		glColor3f(1.0f, 1.0f, 1.0f);
		aux = inicializarObjeto(moverX, moverY, moverZ, rotarX, rotarY, rotarZ, objetos[objetos.size() - 1].sx,
			objetos[objetos.size() - 1].sy, objetos[objetos.size() - 1].sz, objetos[objetos.size() - 1].listaRender);

		for (i = 0; i < objetos.size() - 1; i++) {
			colision = (comprobarColision(aux, objetos[i]));
			if (colision == TRUE) {
				modColExtern();
				break;
			}
		}
		if (colision == FALSE) {
			modColObjeto();
		}

		debuxarObjeto(objetos[objetos.size() - 1]);

		glPopMatrix();
	}

	glFlush();
	glutSwapBuffers();

}

void crearMenu(int item) {
	switch (item) {
	case MENU_PRINCIPAL:
		break;
	case MENU_OBJETOS:
		break;
	case MENU_TEXTURAS:
		break;
	case MENU_ILUMINACION:
		break;
	case CUBO:
		//Estas asignacions son para que apareza no origen, temos que decidir si facer asi ou que apareza superposto
		//ao ultimo objeto
		moverX = 0.0f;
		moverY = 0.0f;
		moverZ = 0.0f;
		rotarX = 0.0f;
		rotarY = 0.0f;
		rotarZ = 0.0f;
		objetos.push_back(inicializarObjeto(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 5.0f, 5.0f, 5.0f, cubo));
		break;
	default:
		break;
	}
	glutPostRedisplay();

	return;
}

void menus() {
	int menuObjetos = glutCreateMenu(crearMenu);

	glutAddMenuEntry("Cubo", CUBO);
	glutAddMenuEntry("Personalizado 1", USER1);
	glutAddMenuEntry("Personalizado 2", USER2);

	//Creacion del submenu de texturas
	int menuTexturas = glutCreateMenu(crearMenu);

	glutAddMenuEntry("Cemento", CEMENTO);
	glutAddMenuEntry("Cristal", CRISTAL);
	glutAddMenuEntry("Hierba", HIERBA);

	//Creacion del submenu de iluminacion
	int menuIluminacion = glutCreateMenu(crearMenu);

	glutAddMenuEntry("Luz 1", LUZ1);
	glutAddMenuEntry("Luz 2", LUZ2);
	glutAddMenuEntry("Luz 3", LUZ3);

	//Creacion del menu principal
	int menuPrincipal = glutCreateMenu(crearMenu);

	glutAddSubMenu("Objetos", MENU_OBJETOS);
	glutAddSubMenu("Texturas", MENU_TEXTURAS);
	glutAddSubMenu("Iluminación", MENU_ILUMINACION);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char **argv) {

	glutInit(&argc, argv);
	//Inicializa la ventana en una determinada posicion
	glutInitWindowPosition(0, 0);
	//Inicializa el tamano de la funcion
	glutInitWindowSize(W_WIDTH, W_HEIGHT);
	//Inicaliza el modeo de display, RGBA y Doble buffer
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	// Nombre de la ventana
	glutCreateWindow("Ventana con el Visual C++ y glut");
	//Incializacion listas
	cuadrado = myCuadrado();
	cubo = myCubo();
	esfera = myEsfera();

	// Detectar profundidad de obxetos y no dibujar caras ocultas
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	// Normaliza las normales
	glEnable(GL_NORMALIZE);

	//Eventos

	glutKeyboardFunc(myTeclado);
	glutSpecialFunc(myTeclasespeciales);
	glutDisplayFunc(myDisplay);

	// Color con el que se limpian los buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//Creacion del submenu de objetos
	menus();

	// Empieza en bucle principal
	glutMainLoop();
	return 0;
}


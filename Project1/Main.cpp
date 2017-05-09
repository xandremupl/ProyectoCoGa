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
int fondo;


#define GL_PI 3.14f


// Ángulos de rotación
//static GLfloat xRot = 0.0f;
//static GLfloat yRot = 0.0f;

//Variables externas
float *rotarX;
float *rotarY;
float *rotarZ;
float *moverX;
float *moverY;
float *moverZ;
float *escalarX;
float *escalarY;
float *escalarZ;

//Funciones externas
extern int myCuadrado();
extern int myCubo();
extern int myEsfera();
extern int myFondo();

enum TIPO_MENU {
	CUBO,
	USER1,
	USER2,
	CEMENTO,
	CRISTAL,
	HIERBA,
	LUZ1,
	LUZ2,
	LUZ3,
	PARAR_MACRO,
	GMUSR1,
	GMUSR2,
	ROJO,
	VERDE,
	AZUL,
	AMARILLO,
	MAGENTA,
	CIAN,
};

typedef struct {
	float px, py, pz; // posicion
	float rx, ry, rz; // angulos de giro
	float sx, sy, sz; // escalado en los tres ejes.
	int listaRender; // lista de render
} ObjBase;

struct Objeto {
	ObjBase base;
	std::vector<Objeto> hijos;
};

typedef struct Objeto Objeto;

std::vector<Objeto> objetos;
Objeto objFondo;
Objeto Pers[2];
GLboolean usrUsado[2];
int grabacion;	//Variable para hacer posibles las grabaciones
int indUser = -1;	//Indice de la macro grabada (-1 significa sin grabar)
float color[3];

ObjBase inicializarObjBase(float px, float py, float pz, float rx, float ry, float rz, float sx,
	float sy, float sz, int listaRender) {
	ObjBase objeto;
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

Objeto inicializarObjeto(ObjBase objBase) {
	Objeto obj;
	obj.base = objBase;
	return(obj);
}

void dibujarObjeto(Objeto objeto) {
	int i;
	ObjBase* base = &(objeto.base);
	
	//Comprobamos si es un nodo hoja o no
	if (objeto.hijos.size() == 0) {	//Obj no tiene hijos
		//Aplicamos transformaciones
		glPushMatrix();
			glTranslatef(base->px, base->py, base->pz);
			glRotatef(base->rx, 1.0f, 0.0f, 0.0f);
			glRotatef(base->ry, 0.0f, 1.0f, 0.0f);
			glRotatef(base->rz, 0.0f, 0.0f, 1.0f);
			glScalef(base->sx, base->sy, base->sz);
			//glScalef(1.0f, 1.0f, 1.0f);
			glCallList(base->listaRender);
		glPopMatrix();
	}
	else {
		for (i = 0; i < objeto.hijos.size(); i++) {
			//Aplicamos transformaciones
			glPushMatrix();
				glTranslatef(base->px, base->py, base->pz);
				glRotatef(base->rx, 1.0f, 0.0f, 0.0f);
				glRotatef(base->ry, 0.0f, 1.0f, 0.0f);
				glRotatef(base->rz, 0.0f, 0.0f, 1.0f);
				glScalef(base->sx, base->sy, base->sz);
				dibujarObjeto(objeto.hijos[i]);
			glPopMatrix();
		}
	}
}

void asignarParametros() {
	moverX = &(objetos[objetos.size() - 1].base.px);
	moverY = &(objetos[objetos.size() - 1].base.py);
	moverZ = &(objetos[objetos.size() - 1].base.pz);
	rotarX = &(objetos[objetos.size() - 1].base.rx);
	rotarY = &(objetos[objetos.size() - 1].base.ry);
	rotarZ = &(objetos[objetos.size() - 1].base.rz);
	escalarX = &(objetos[objetos.size() - 1].base.sx);
	escalarY = &(objetos[objetos.size() - 1].base.sy);
	escalarZ = &(objetos[objetos.size() - 1].base.sz);
}

void cambiarColorFondo(float red, float green, float blue) {
	color[0] = red;
	color[1] = green;
	color[2] = blue;
}

// Funcion de dibukop
void myDisplay(void) {
	int i;


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the window with current clearing color

	myCamara();

	// Incluye una matriz de rotacion en todos los puntos que se dibujen
	glMatrixMode(GL_MODELVIEW);
	// Inicializamos la matriz del modelo a la identidad
	glLoadIdentity();

	glPushMatrix();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(color[0], color[1], color[2]);
	glTranslatef(0.0f, 0.0f, objFondo.base.pz);
	glCallList(fondo);

	glPopMatrix();


	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (objetos.size() > 0) {
		for (i = 0; i < objetos.size() - 1; i++) {
			glPushMatrix();

			glColor3f(1.0f, 1.0f, 1.0f);
			dibujarObjeto(objetos[i]);

			glPopMatrix();
		}

		glPushMatrix();

		glColor3f(1.0f, 1.0f, 1.0f);
		asignarParametros();
		dibujarObjeto(objetos[objetos.size() - 1]);
		glPopMatrix();
	}

	glFlush();
	glutSwapBuffers();

}

void colocarPersonalizado(int indice) {
	if (usrUsado[indice] == TRUE) {		//Sirve para comprobar si Pers[indice] tiene algo
		objetos.push_back(Pers[indice]);	//Colocamos objeto personalizado [indice]
	}
}

void crearMenu(int item) {
	switch (item) {
	case USER1:
		colocarPersonalizado(0);
		break;
	case USER2:
		colocarPersonalizado(1);
		break;
	case CUBO:
		//Estas asignacions son para que apareza no origen, temos que decidir si facer asi ou que apareza superposto
		//ao ultimo objeto
		objetos.push_back(inicializarObjeto(inicializarObjBase(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 1.0f, cubo)));
		break;
	case PARAR_MACRO:
		if (indUser <= -1) {
			return;
		}
		Pers[indUser] = inicializarObjeto(inicializarObjBase(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, NULL));
		//Itera por los objetos añadidos al vector objetos para guardarlos en la superestructura (Objeto Pers) y hace pop de ellos
		for (objetos.size(); objetos.size() > grabacion;) {
			Pers[indUser].hijos.push_back(objetos[objetos.size()-1]);	//Metemos el objeto mas externo en Pers
			objetos.pop_back();	//Y lo eliminamos de objetos
		}
		usrUsado[indUser] = TRUE; //Indica que Pers[indUser] tiene algo
		indUser = -1;	//Pone la variable de macro a "Sin grabacion"
		break;
	case GMUSR1:
		indUser = 0;	//Guardo el indice de la macro que se esta grabando
		grabacion = objetos.size();	//Guardamos el numero de objetos que habia en escena cuando empezamos a grabar
		break;
	case GMUSR2:
		indUser = 1;
		grabacion = objetos.size();	//Guardamos el numero de objetos que habia en escena cuando empezamos a grabar
		break;
	case ROJO:
		cambiarColorFondo(1.0f, 0.0f, 0.0f);
		break;
	case VERDE:
		cambiarColorFondo(0.0f, 1.0f, 0.0f);
		break;
	case AZUL:
		cambiarColorFondo(0.0f, 0.0f, 1.0f);
		break;
	case AMARILLO:
		cambiarColorFondo(1.0f, 1.0f, 0.0f);
		break;
	case MAGENTA:
		cambiarColorFondo(1.0f, 0.0f, 1.0f);
		break;
	case CIAN:
		cambiarColorFondo(0.0f, 1.0f, 1.0f);
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

	//Creacion del menu de grabacion
	int grabarMacro = glutCreateMenu(crearMenu);

	glutAddMenuEntry("Personalizado 1", GMUSR1);
	glutAddMenuEntry("Personalizado 2", GMUSR2);

	//Creacion del menu de macros
	int menuMacro = glutCreateMenu(crearMenu);

	glutAddSubMenu("Grabar", grabarMacro);
	glutAddMenuEntry("Parar", PARAR_MACRO);

	//Creacion del menu del fondo
	int menuFondo = glutCreateMenu(crearMenu);

	glutAddMenuEntry("Rojo", ROJO);
	glutAddMenuEntry("Verde", VERDE);
	glutAddMenuEntry("Azul", AZUL);
	glutAddMenuEntry("Amarillo", AMARILLO);
	glutAddMenuEntry("Magenta", MAGENTA);
	glutAddMenuEntry("Cian", CIAN);


	//Creacion del menu principal
	int menuPrincipal = glutCreateMenu(crearMenu);

	glutAddSubMenu("Objetos", menuObjetos);
	glutAddSubMenu("Texturas", menuTexturas);
	glutAddSubMenu("Iluminación", menuIluminacion);
	glutAddSubMenu("Macros", menuMacro);
	glutAddSubMenu("Fondo", menuFondo);
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
	fondo = myFondo();

	//Inicializacion fondo
	objFondo = inicializarObjeto(inicializarObjBase(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, fondo));
	cambiarColorFondo(1.0f, 0.0f, 0.0f);

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


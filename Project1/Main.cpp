#include <windows.h>	//Inclusion de variables de windows

#include <glut.h>	//Inclusion de GLUT
#include <GL/gl.h>		//Inclusion de GL		 	
#include <GL/glu.h>	
#include <vector>
#include <stdio.h>
#include <math.h>	//Inclusion de librerias auxiliares	
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
extern void myCamara();
extern void myTeclado(unsigned char tras, int x, int y);
extern void myTeclasespeciales(int cursor, int x, int y);
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

}

Objeto base;
Objeto esfBase;
Objeto mastil;
Objeto esfTope;
Objeto tope;

// Funcion de dibukop
void myDisplay(void) {



	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the window with current clearing color

	myCamara();

	// Incluye una matriz de rotacion en todos los puntos que se dibujen
	glMatrixMode(GL_MODELVIEW);
	// Inicializamos la matriz del modelo a la identidad
	glLoadIdentity();

	glPushMatrix();



	glPopMatrix();

	glFlush();
	glutSwapBuffers();

}

void crearMenu(int);

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
	// llamada a la funcion de dibujo
	//Incializacion listas
	cuadrado = myCuadrado();
	cubo = myCubo();
	esfera = myEsfera();

	//Inicializacion objetos
	/*base = inicializarObjeto(0.0f, 0.0f, -30.0f, 0.0f, 0.0f, 60.0f, 20.0f, 60.0f, cubo);
	esfBase = inicializarObjeto(0.0f, 7.0f, 0.0f, 0.0f, 0.0f, 10.0f, 10.0f, 10.0f, esfera);
	mastil = inicializarObjeto(0.0f, 20.0f, -2.5f, 0.0f, 0.0f, 4.0f, 50.0f, 4.0f, cubo);
	esfTope = inicializarObjeto(0.0f, 43.0f, 0.0f, 0.0f, 0.0f, 6.0f, 6.0f, 6.0f, esfera);
	tope = inicializarObjeto(0.0f, 10.0f, -1.0f, 0.0f, 0.0f, 2.0f, 20.0f, 2.0f, cubo);*/

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
	// Empieza en bucle principal
	glutMainLoop();
	return 0;
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
		objetos.push_back(inicializarObjeto(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, glCallList(cubo)));
	default:
		break;
	}
	glutPostRedisplay();

	return;
}
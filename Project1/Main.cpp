#include <windows.h>	//Inclusion de variables de windows

#include <glut.h>	//Inclusion de GLUT
#include <GL/gl.h>		//Inclusion de GL		 	
#include <GL/glu.h>	
#include <vector>
#include <stdio.h>
#include <SOIL.h>
#include <GLAUX.H>

#include <math.h>	//Inclusion de librerias auxiliares	
#include "Camara_teclado.h"
int W_WIDTH = 500;	 //Ancho de la ventana
int W_HEIGHT = 500;		//Alto de la ventana

int cuadrado;
int cubo;
int esfera;
int fondo;


#define GL_PI 3.14f


// Ángulos de rotación
//static GLfloat xRot = 0.0f;
//static GLfloat yRot = 0.0f;

float *rotarX;
float *rotarY;
float *rotarZ;
float *moverX;
float *moverY;
float *moverZ;
float *escalarX;
float *escalarY;
float *escalarZ;
GLboolean luzSeparado[3] = { FALSE, FALSE, FALSE };

extern GLboolean luz;

//Funciones externas
//extern AUX_RGBImageRec *Carga_BMP(char *Nome_ficheiro);
//extern void Libera_BMP(AUX_RGBImageRec *TextureImage);
extern int myCuadrado();
extern int myCubo();
extern int myEsfera();
extern int myFondo();

enum TIPO_MENU {
	CUBO,
	ESFERA,
	FONDO,
	USER1,
	USER2,
	USER3,
	USER4,
	USER5,
	ELIMINAR,
	NONE_TEXTURE,
	COBBLESTONE,
	BRICK,
	CRISTAL,
	HIERBA,
	LUZ1,
	LUZ2,
	LUZ3,
	PARAR_MACRO,
	GMUSR1,
	GMUSR2,
	GMUSR3,
	GMUSR4,
	GMUSR5,
	ROJO,
	VERDE,
	AZUL,
	AMARILLO,
	MAGENTA,
	CIAN,
	BLANCO,
	SKY,
	SPACE,
};

typedef struct {
	float px, py, pz; // posicion
	float rx, ry, rz; // angulos de giro
	float sx, sy, sz; // escalado en los tres ejes.
	float r, g, b; // colores
	int listaRender; // lista de render
	GLuint textura;	//Textura
} ObjBase;

struct Objeto {
	ObjBase base;
	std::vector<Objeto> hijos;
};

typedef struct Objeto Objeto;

std::vector<Objeto> objetos;
Objeto objFondo;
Objeto Pers[5];
GLboolean usrUsado[5];
int grabacion;	//Variable para hacer posibles las grabaciones
int indUser = -1;	//Indice de la macro grabada (-1 significa sin grabar)
float color[3];

void Carga_Texturas(GLuint *tex, char* ruta) {

	*tex = SOIL_load_OGL_texture(ruta, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glBindTexture(GL_TEXTURE_2D, *tex);
}

ObjBase inicializarObjBase(float px, float py, float pz, float rx, float ry, float rz, float sx,
	float sy, float sz, float r, float g, float b, int listaRender) {
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
	objeto.r = r;
	objeto.g = g;
	objeto.b = b;
	objeto.listaRender = listaRender;
	return(objeto);
}

Objeto inicializarObjeto(ObjBase objBase) {
	Objeto obj;
	obj.base = objBase;
	return(obj);
}

void changeSize(GLint newWidth, GLint newHeight) {
	W_WIDTH = newWidth;
	W_HEIGHT = newHeight;

	glViewport(0, 0, W_WIDTH, W_HEIGHT);
	glutPostRedisplay();
}

void dibujarObjeto(Objeto objeto) {
	int i;
	ObjBase* base = &(objeto.base);
	
	//Comprobamos si es un nodo hoja o no
	if (objeto.hijos.size() == 0) {	//Obj no tiene hijos
		//Aplicamos transformaciones
		glPushMatrix();
			glBindTexture(GL_TEXTURE_2D, objeto.base.textura);
			glColor3f(base->r, base->g, base->b);
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

void cambiarColorObjeto(Objeto *objeto, float red, float green, float blue) {
	int i;
	if (objeto->hijos.size() == 0) {	//Obj no tiene hijos
		objeto->base.r = red;
		objeto->base.g = green;
		objeto->base.b = blue;
	}
	else {
		for (i = 0; i < objeto->hijos.size(); i++) {
			cambiarColorObjeto(&(objeto->hijos[i]), red, green, blue);
		}
	}
}

void activarLuz() {
	if (luz) {
		glEnable(GL_LIGHTING);
	}
	else {
		glDisable(GL_LIGHTING);
	}
	if (luzSeparado[0]) {
		glEnable(GL_LIGHT0);
	}
	else {
		glDisable(GL_LIGHT0);
	}
	if (luzSeparado[1]) {
		glEnable(GL_LIGHT1);
	}
	else {
		glDisable(GL_LIGHT1);
	}
	if (luzSeparado[2]) {
		glEnable(GL_LIGHT2);
	}
	else {
		glDisable(GL_LIGHT2);
	}
}

// Funcion de dibukop
void myDisplay(void) {
	int i;


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the window with current clearing color

	myCamara();

	activarLuz();

	// Incluye una matriz de rotacion en todos los puntos que se dibujen
	glMatrixMode(GL_MODELVIEW);
	// Inicializamos la matriz del modelo a la identidad
	glLoadIdentity();

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
	case LUZ1:
		if (luzSeparado[0]) {
			luzSeparado[0] = FALSE;
		}
		else {
			luzSeparado[0] = TRUE;
		}
		break;
	case LUZ2:
		if (luzSeparado[1]) {
			luzSeparado[1] = FALSE;
		}
		else {
			luzSeparado[1] = TRUE;
		}
		break;
	case LUZ3:
		if (luzSeparado[2]) {
			luzSeparado[2] = FALSE;
		}
		else {
			luzSeparado[2] = TRUE;
		}
		break;
	case USER1:
		colocarPersonalizado(0);
		break;
	case USER2:
		colocarPersonalizado(1);
		break;
	case USER3:
		colocarPersonalizado(2);
		break;
	case USER4:
		colocarPersonalizado(3);
		break;
	case USER5:
		colocarPersonalizado(4);
		break;
	case CUBO:
		objetos.push_back(inicializarObjeto(inicializarObjBase(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, cubo)));
		break;
	case ESFERA:
		objetos.push_back(inicializarObjeto(inicializarObjBase(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			5.0f, 5.0f, 5.0f, 1.0f, 1.0f, 1.0f, esfera)));
		break;
	case FONDO:
		objetos.push_back(inicializarObjeto(inicializarObjBase(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, fondo)));
		break;
	case ELIMINAR:
		objetos.pop_back();
		break;
	case PARAR_MACRO:
		if (indUser <= -1) {
			return;
		}

		if (objetos.size() > grabacion){	//Comprueba si se hicieron cambios
			Pers[indUser] = inicializarObjeto(inicializarObjBase(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, NULL));
			//Itera por los objetos añadidos al vector objetos para guardarlos en la superestructura (Objeto Pers) y hace pop de ellos
			for (objetos.size(); objetos.size() > grabacion;) {
				Pers[indUser].hijos.push_back(objetos[objetos.size() - 1]);	//Metemos el objeto mas externo en Pers
				objetos.pop_back();	//Y lo eliminamos de objetos
			}
			usrUsado[indUser] = TRUE; //Indica que Pers[indUser] tiene algo
		}
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
	case GMUSR3:
		indUser = 2;
		grabacion = objetos.size();	//Guardamos el numero de objetos que habia en escena cuando empezamos a grabar
		break;
	case GMUSR4:
		indUser = 3;
		grabacion = objetos.size();	//Guardamos el numero de objetos que habia en escena cuando empezamos a grabar
		break;
	case GMUSR5:
		indUser = 4;
		grabacion = objetos.size();	//Guardamos el numero de objetos que habia en escena cuando empezamos a grabar
		break;
	case ROJO:
		cambiarColorObjeto(&(objetos[objetos.size()-1]), 1.0f, 0.0f, 0.0f);
		break;
	case VERDE:
		cambiarColorObjeto(&(objetos[objetos.size() - 1]), 0.0f, 1.0f, 0.0f);
		break;
	case AZUL:
		cambiarColorObjeto(&(objetos[objetos.size() - 1]), 0.0f, 0.0f, 1.0f);
		break;
	case AMARILLO:
		cambiarColorObjeto(&(objetos[objetos.size() - 1]), 1.0f, 1.0f, 0.0f);
		break;
	case MAGENTA:
		cambiarColorObjeto(&(objetos[objetos.size() - 1]), 1.0f, 0.0f, 1.0f);
		break;
	case CIAN:
		cambiarColorObjeto(&(objetos[objetos.size() - 1]), 0.0f, 1.0f, 1.0f);
		break;
	case BLANCO:
		cambiarColorObjeto(&(objetos[objetos.size() - 1]), 1.0f, 1.0f, 1.0f);
		break;
	case NONE_TEXTURE:
		objetos[objetos.size() - 1].base.textura = 0;
		break;
	case COBBLESTONE:
		Carga_Texturas(&(objetos[objetos.size() - 1].base.textura), "./Texturas/cobblestone.png");
		break;
	case BRICK:
		Carga_Texturas(&(objetos[objetos.size() - 1].base.textura), "./Texturas/brick.png");
		break;
	case SKY:
		Carga_Texturas(&(objetos[objetos.size() - 1].base.textura), "./Texturas/sky.png");
		break;
	case HIERBA:
		Carga_Texturas(&(objetos[objetos.size() - 1].base.textura), "./Texturas/hierba.png");
		break;
	case SPACE:
		Carga_Texturas(&(objetos[objetos.size() - 1].base.textura), "./Texturas/space.png");
		break;
	default:
		break;
	}
	glutPostRedisplay();

	return;
}

void Iluminacion() {
	//Variables de luz


	GLfloat Ambient0[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat Position0[] = { -1.5f, 1.0f, -4.0f, 1.0f };
	GLfloat Diffuse0[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	GLfloat Position1[] = { 0.0f, 0.0f, 20.0f, 1.0f };
	GLfloat SpotDir1[] = { 0.0f, 0.0f, -20.0f };
	GLfloat Diffuse1[] = { 0.0f, 1.0f, 0.0f, 1.0f };

	GLfloat Position2[] = { 0.0f, 20.0f, 0.0f, 1.0f };
	GLfloat Diffuse2[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	GLfloat SpotDir2[] = { 0.0f, -20.0f, 0.0f };


	//Definimos las luces
	glLightfv(GL_LIGHT0, GL_AMBIENT, Ambient0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Diffuse0);
	glLightfv(GL_LIGHT0, GL_POSITION, Position0);

	glLightfv(GL_LIGHT1, GL_DIFFUSE, Diffuse1);
	glLightfv(GL_LIGHT1, GL_POSITION, Position1);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, SpotDir1);

	glLightfv(GL_LIGHT2, GL_DIFFUSE, Diffuse2);
	glLightfv(GL_LIGHT2, GL_POSITION, Position2);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, SpotDir2);

}

void menus() {
	int menuObjetos = glutCreateMenu(crearMenu);

	glutAddMenuEntry("Cubo", CUBO);
	glutAddMenuEntry("Esfera", ESFERA);
	glutAddMenuEntry("Fondo", FONDO);
	glutAddMenuEntry("Personalizado 1", USER1);
	glutAddMenuEntry("Personalizado 2", USER2);
	glutAddMenuEntry("Personalizado 3", USER3);
	glutAddMenuEntry("Personalizado 4", USER4);
	glutAddMenuEntry("Personalizado 5", USER5);
	glutAddMenuEntry("Eliminar objeto", ELIMINAR);

	//Creacion del submenu de texturas
	int menuTexturas = glutCreateMenu(crearMenu);

	glutAddMenuEntry("Ninguna", NONE_TEXTURE);
	glutAddMenuEntry("Cobblestone", COBBLESTONE);
	glutAddMenuEntry("Bricks", BRICK);
	glutAddMenuEntry("Hierba", HIERBA);
	glutAddMenuEntry("Sky", SKY);
	glutAddMenuEntry("Space", SPACE);

	//Creacion del submenu de iluminacion
	int menuIluminacion = glutCreateMenu(crearMenu);

	glutAddMenuEntry("Luz Ambiente", LUZ1);
	glutAddMenuEntry("Luz Verde", LUZ2);
	glutAddMenuEntry("Luz Azul", LUZ3);

	//Creacion del menu de grabacion
	int grabarMacro = glutCreateMenu(crearMenu);

	glutAddMenuEntry("Personalizado 1", GMUSR1);
	glutAddMenuEntry("Personalizado 2", GMUSR2);
	glutAddMenuEntry("Personalizado 3", GMUSR3);
	glutAddMenuEntry("Personalizado 4", GMUSR4);
	glutAddMenuEntry("Personalizado 5", GMUSR5);

	//Creacion del menu de macros
	int menuMacro = glutCreateMenu(crearMenu);

	glutAddSubMenu("Grabar", grabarMacro);
	glutAddMenuEntry("Parar", PARAR_MACRO);

	//Creacion del menu del color
	int menuColor = glutCreateMenu(crearMenu);

	glutAddMenuEntry("Rojo", ROJO);
	glutAddMenuEntry("Verde", VERDE);
	glutAddMenuEntry("Azul", AZUL);
	glutAddMenuEntry("Amarillo", AMARILLO);
	glutAddMenuEntry("Magenta", MAGENTA);
	glutAddMenuEntry("Cian", CIAN);
	glutAddMenuEntry("Blanco", BLANCO);

	

	//Creacion del menu principal
	int menuPrincipal = glutCreateMenu(crearMenu);

	glutAddSubMenu("Objetos", menuObjetos);
	glutAddSubMenu("Texturas", menuTexturas);
	glutAddSubMenu("Iluminación", menuIluminacion);
	glutAddSubMenu("Macros", menuMacro);
	glutAddSubMenu("Color", menuColor);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void init() {
	
}

int myFondo() {
	int indice;
	indice = glGenLists(1);
	glNewList(indice, GL_COMPILE);

	glScalef(250.0f, 250.0f, 250.0f);

	// Cara 1 (frontal) 
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(.5f, .5f, .5f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(.5f, -.5f, .5f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-.5f, -.5f, .5f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-.5f, .5f, .5f);
	glEnd();

	//Cara 2 (izquierda)
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-.5f, .5f, .5f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-.5f, -.5f, .5f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(-.5f, -.5f, -.5f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-.5f, .5f, -.5f);
	glEnd();

	//Cara 3 (trasera)
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-.5f, -.5f, -.5f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(.5f, -.5f, -.5f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(.5f, .5f, -.5f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-.5f, .5f, -.5f);
	glEnd();

	//Cara 4 (derecha)
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(.5f, .5f, -.5f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(.5f, -.5f, -.5f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(.5f, -.5f, .5f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(.5f, .5f, .5f);
	glEnd();

	//Cara 6 (inferior)
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-.5f, -.5f, .5f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(.5f, -.5f, .5f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(.5f, -.5f, -.5f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-.5f, -.5f, -.5f);
	glEnd();


	glEndList();

	return indice;
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
	glutCreateWindow("3D Composite-Builder");
	//Incializacion listas
	cuadrado = myCuadrado();
	cubo = myCubo();
	esfera = myEsfera();
	fondo = myFondo();

	Iluminacion();

	//Inicializacion fondo
	
	objetos.push_back(inicializarObjeto(inicializarObjBase(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f, fondo)));

	// Detectar profundidad de obxetos y no dibujar caras ocultas
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	// Normaliza las normales
	glEnable(GL_NORMALIZE);

	//Eventos

	glutKeyboardFunc(myTeclado);
	glutReshapeFunc(changeSize);
	glutSpecialFunc(myTeclasespeciales);
	glutDisplayFunc(myDisplay);

	// Color con el que se limpian los buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//Creacion del submenu de objetos
	init();
	menus();

	// Empieza en bucle principal
	glutMainLoop();
	return 0;
}


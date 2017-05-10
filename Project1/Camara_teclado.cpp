#include <windows.h>	//Inclusion de variables de windows

#include <glut.h>	//Inclusion de GLUT
#include <GL/gl.h>		//Inclusion de GL		 	
#include <GL/glu.h>	
#include <stdio.h>

#include <math.h>	//Inclusion de librerias auxiliares
#include "Camara_teclado.h"

extern float *rotarX;
extern float *rotarY;
extern float *rotarZ;
extern float *moverX;
extern float *moverY;
extern float *moverZ;
extern float *escalarX;
extern float *escalarY;
extern float *escalarZ;

float alpha = 0;
float beta = 0;

void myCamara() {

	//Configuración de la matriz de proyeccion
	glMatrixMode(GL_PROJECTION);
	//La ponemos a uno
	glLoadIdentity();
	glOrtho(-100.0, 100.0f, -100.0, 100.0f, -100.0, 1000.0f);
	gluLookAt(((float)DISTANCIA*(float)sin(alpha)*cos(beta)), ((float)DISTANCIA*(float)sin(beta)), ((float)DISTANCIA*cos(alpha)*cos(beta)), 0, 0, 0, 0, 1, 0);

}

void myTeclado(unsigned char tras, int x, int y)
{
	switch (tras) {
	case 'a':
		*rotarZ -= INCRANG;
		break;
	case 's':
		*rotarX -= INCRANG;
		break;
	case 'd':
		*rotarZ += INCRANG;
		break;
	case 'w':
		*rotarX += INCRANG;
		break;
	case 'q':
		*rotarY -= INCRANG;
		break;
	case 'e':
		*rotarY += INCRANG;
		break;
	case 'j':
		*moverX -= INCMOV;
		break;
	case 'k':
		*moverY -= INCMOV;
		break;
	case 'l':
		*moverX += INCMOV;
		break;
	case 'i':
		*moverY += INCMOV;
		break;
	case 'u':
		*moverZ -= INCMOV;
		break;
	case 'o':
		*moverZ += INCMOV;
		break;
	case 'f':
		*escalarX -= INCESC;
		break;
	case 'g':
		*escalarY -= INCESC;
		break;
	case 'h':
		*escalarZ -= INCESC;
		break;
	case 'r':
		*escalarX += INCESC;
		break;
	case 't':
		*escalarY += INCESC;
		break;
	case 'y':
		*escalarZ += INCESC;
		break;
	case 27:
		exit(0);
		break;
	default:
		break;
	}
	// Se se modificou algo redebúxase
	glutPostRedisplay();
}

/**************************************
FUNCION DE CONTROL DE TECLAS ESPECIAIS:
**************************************
*/
void myTeclasespeciales(int cursor, int x, int y)
{
	switch (cursor)
	{
		//Traslaciones:
	case GLUT_KEY_F1:
		break;
	case GLUT_KEY_F2:

		break;
	case GLUT_KEY_F3:

		break;
	case GLUT_KEY_F4:
		break;
	case GLUT_KEY_F5:

		break;
	case GLUT_KEY_F6:

		break;
		//Giros:
	case GLUT_KEY_UP:
		beta -= INCREMENTO;
		break;
	case GLUT_KEY_DOWN:
		beta += INCREMENTO;
		break;
	case GLUT_KEY_RIGHT:
		alpha -= INCREMENTO;
		break;
	case GLUT_KEY_LEFT:
		alpha += INCREMENTO;
		break;
	default:
		break;
	}

	if (alpha >= PI*2.0 && alpha <= 0) alpha = 0;
	if (beta >= PI*2.0 && beta <= 0) beta = 0; //hay que repasarlo para evitar el salto


	glutPostRedisplay();
}
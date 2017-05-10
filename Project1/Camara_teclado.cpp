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
GLboolean perspectiva[5] = { FALSE, FALSE, FALSE, TRUE, FALSE };
GLboolean luz = FALSE;

void myCamara() {

	//Configuración de la matriz de proyeccion
	glMatrixMode(GL_PROJECTION);
	//La ponemos a uno
	glLoadIdentity();
	if (perspectiva[4]) {
		gluPerspective(65, 1, 12, 1000);
		gluLookAt(((float)DISTANCIA * 9 * (float)sin(alpha)*cos(beta)), ((float)DISTANCIA * 9 * (float)sin(beta)),
			((float)DISTANCIA * 9 * cos(alpha)*cos(beta)), 0, 0, 0, 0, 1, 0);
	}
	else {
		glOrtho(-200.0, 200.0f, -200.0, 200.0f, -200.0, 400.0f);
		gluLookAt(((float)DISTANCIA * (float)sin(alpha)*cos(beta)), ((float)DISTANCIA * (float)sin(beta)),
			((float)DISTANCIA * cos(alpha)*cos(beta)), 0, 0, 0, 0, 1, 0);
	}
	

}

void cambiarPerspectiva(GLboolean planta, GLboolean alzado, GLboolean perfil, GLboolean ortho, GLboolean iso) {
	perspectiva[0] = planta;
	perspectiva[1] = alzado;
	perspectiva[2] = perfil;
	perspectiva[3] = ortho;
	perspectiva[4] = iso;
}

void myTeclado(unsigned char tras, int x, int y)
{
	switch (tras) {
	case '1':
		cambiarPerspectiva(TRUE, FALSE, FALSE, FALSE, FALSE);
		alpha = 0.0f;
		beta = 1.56999;
		break;
	case '2':
		cambiarPerspectiva(FALSE, TRUE, FALSE, FALSE, FALSE);
		alpha = 0.0f;
		beta = 0.0f;
		break;
	case '3':
		cambiarPerspectiva(FALSE, FALSE, TRUE, FALSE, FALSE);
		alpha = 1.56999;
		beta = 0.0f;
		break;
	case '4':
		cambiarPerspectiva(FALSE, FALSE, FALSE, TRUE, FALSE);
		break;
	case '5':
		cambiarPerspectiva(FALSE, FALSE, FALSE, FALSE, TRUE);
		break;
	case '-':
		*escalarX -= INCESC;
		*escalarY -= INCESC;
		*escalarZ -= INCESC;
		break;
	case '+':
		*escalarX += INCESC;
		*escalarY += INCESC;
		*escalarZ += INCESC;
		break;
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
	case 'm':
		if (!luz) {
			luz = TRUE;
		}
		else {
			luz = FALSE;
		}
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
	if ((!perspectiva[0]) && (!perspectiva[1]) && (!perspectiva[2])) {
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
			printf("Beta: %f\n", beta);
			break;
		case GLUT_KEY_DOWN:
			beta += INCREMENTO;
			printf("Beta: %f\n", beta);
			break;
		case GLUT_KEY_RIGHT:
			alpha -= INCREMENTO;
			printf("Alfa: %f\n", alpha);
			break;
		case GLUT_KEY_LEFT:
			alpha += INCREMENTO;
			printf("Alfa: %f\n", alpha);
			break;
		default:
			break;
		}

		if (alpha >= PI*2.0 && alpha <= 0) alpha = 0;
		if (beta >= PI*2.0 && beta <= 0) beta = 0; //hay que repasarlo para evitar el salto
	}

	glutPostRedisplay();
}
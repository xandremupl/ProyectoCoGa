#include <windows.h>
#include <GL/gl.h>
#include <GLAUX.h>
#include <stdio.h>
#include<SOIL.h>

AUX_RGBImageRec *Carga_BMP(char *Nome_ficheiro) {
	FILE *File=NULL;

	if (!Nome_ficheiro){
		return NULL;
		}
	
	fopen_s(&File,Nome_ficheiro,"r");

	if (File){
		fclose(File);
		return auxDIBImageLoad(Nome_ficheiro);
	}
	else {
		printf("NO CARGA TEXTURA\n");
	}
	return NULL;
}

void Libera_BMP(AUX_RGBImageRec *TextureImage) {
	if (TextureImage) {
		if (TextureImage->data){
			free(TextureImage->data);					
			}
		free(TextureImage);							
		}
}



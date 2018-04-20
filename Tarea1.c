#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct { //Struct dado en el PDF
  char sigla[7];
  int semestre; /* 1er o 2do semestre*/
} curso;

typedef struct { //Struct dado en el PDF
  char rolEstudiante[12];
  char siglaCurso[7];
  int nota;
} nota;

typedef struct{ //Struct de tipo TipoAlumno que contiene:
  char rolAlumno[12]; //arreglo de char con rol del alumno
	char NombreAl[15]; // arreglo de char con nombre del alumno
	char ApellidoAl[15]; //arreglo de char con apellido del alumno
	int annoIng; //entero con el año de ingreso del alunmo
} TipoAlumnos;

void * LeerBinario(char * nomarch, int*m){ //Función que recibe como parámetro
  #define N curso //el nombre del archivo y un puntero entero, y retorna un puntero
	if (nomarch != "cursos.dat"){ //a un array de tipo 
    #undef N
    #define N nota
	}
  FILE *fp = fopen(nomarch,"r");
  int i;
  if (fp==NULL){
    printf("Error al abrir el archivo\n");
    exit(1);
  }
  fread(&i,sizeof(int),1,fp);
  N *ptr = (N *)malloc(sizeof(N)*i);
  fread(ptr,sizeof(N),i,fp);
  fclose(fp);
  *m = i;
  return ptr;
}
/* */
void * LeerAscii(char * nomarch, int *m){
	FILE *fp = fopen(nomarch,"r");
	int i;
	if (fp==NULL){
			printf("Error al abrir el archivo\n");
			exit(1);
	}
	fscanf(fp,"%d",&i);
	TipoAlumnos *ptr = (TipoAlumnos *)malloc(sizeof(TipoAlumnos)*i);
  for (int j = 0; j < i; j++) {
     fscanf(fp,"%s %s %s %d",ptr[j].rolAlumno,ptr[j].NombreAl,ptr[j].ApellidoAl,&(ptr[j].annoIng));
  }
  fclose(fp);
  *m = i;
	return ptr;
}

int main(){ // NO OLVIDAR FREE A LA MEMORIA
    int cantidadRamos;// cantidad de ramos
    int cantidadNotas;//cantidad de notas
    int cantidadRoles;//cantidad de personas
    curso *ramos=LeerBinario("cursos.dat", &cantidadRamos);
    nota *notas=LeerBinario("notas.dat", &cantidadNotas);
		TipoAlumnos *alumno=LeerAscii("alumnos.txt",&cantidadRoles);
    typedef struct{
      char ramo[7];
      int nota;
    }TipoNotaRamo;
    FILE *aprobadosS1 = fopen("aprobados-s1.txt","w");
    FILE *aprobadosS2 = fopen("aprobados-s2.txt","w");
    for (int l = 0; l < cantidadRoles ; l++)
    {
      TipoNotaRamo ramosSemestre1[cantidadRamos],ramosSemestre2[cantidadRamos];
      bool aprobadoSem1=true;
      bool aprobadoSem2=true;
      int cant1=0,cant2=0;
      for (int cont=0 ;cont < cantidadNotas; cont++)
      {
        if (strcmp(notas[cont].rolEstudiante,alumno[l].rolAlumno)==0){
          for (int jkl=0;jkl<cantidadRamos;jkl++){
            if (strcmp(notas[cont].siglaCurso,ramos[jkl].sigla)==0){
              if (ramos[jkl].semestre==1){
                strcpy(ramosSemestre1[cant1].ramo,notas[cont].siglaCurso);
                ramosSemestre1[cant1].nota=notas[cont].nota;
                cant1+=1;
                if (notas[cont].nota < 55){
                  aprobadoSem1=false;
                }
              }
              else {
                strcpy(ramosSemestre2[cant2].ramo,notas[cont].siglaCurso);
                ramosSemestre2[cant2].nota=notas[cont].nota;
                cant2+=1;
                if (notas[cont].nota < 55){
                  aprobadoSem2=false;
                }
              }
            }
          }
        }
      }
      if ((aprobadoSem1) && (cant1 > 0)) {
        fprintf(aprobadosS1, "%s %s %s ",alumno[l].NombreAl,alumno[l].ApellidoAl,alumno[l].rolAlumno);
        for (int estamosllegandoalfinal = 0; estamosllegandoalfinal < cant1; estamosllegandoalfinal++) {
          fprintf(aprobadosS1, "%s %d ", ramosSemestre1[estamosllegandoalfinal].ramo,ramosSemestre1[estamosllegandoalfinal].nota );
        }
        fprintf(aprobadosS1, "\n");
      }


      if ((aprobadoSem2) && (cant2 > 0)) {
          fprintf(aprobadosS2, "%s %s %s ",alumno[l].NombreAl,alumno[l].ApellidoAl,alumno[l].rolAlumno);
          for (int estamosllegandoalfinal = 0; estamosllegandoalfinal < cant2; estamosllegandoalfinal++) {
            fprintf(aprobadosS2, "%s %d ", ramosSemestre2[estamosllegandoalfinal].ramo,ramosSemestre2[estamosllegandoalfinal].nota );
          }
          fprintf(aprobadosS2, "\n" );
      }
    }
fclose(aprobadosS1);
fclose(aprobadosS2);
return 0;
}

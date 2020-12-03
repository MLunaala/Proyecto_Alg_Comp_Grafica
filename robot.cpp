//Desarrollado por Mod y compañia//
#include <iostream>
#include <stdlib.h>
#include <GL/glut.h>

using namespace std;
// Variables globales
int accion=0; //ingreso por teclado
//variables de giro: 
float angle[3][8]= //almacena los ángulos en una matriz siendo: 
	{{0,0.1,0,0.1,0,0.1,0,0.1}, //los números pares representan el ángulo de rotación
	{0,0.1,0,0.1,0,0.1,0,0.1}, //los números impares representan el giro adelante y atrás
	{0,0.1,0,0.1,0,0.1,0,0.1}}; //los [0][x] para brazos, los [1][x] para piernas y los [2][x] para traslaci
int   lastx = 0,  lasty = 0;    // puntos de posicion para el mouse
float rotx = 10.0, roty = -30.0; //Rotacion sobre el x y y con el click
float zoom = 1.0; 				//variable para el zoom de la cámara
float tx = 0.0,ty = 0.0, tz = 0.0;;     // translada con respecto a x, y,z
bool  buttonPressed[3];         // matriz que representa el estado de mouse
 
//---------------------------------------------------------------------------
//configuracione iniciales
void graphicsInit( void ){
// Fuente de luz inicial
    const GLfloat light_ambient[]  = { 0.1, 0.1, 0.1, 1.0 };
    const GLfloat light_diffuse[]  = { 1.0, 1.0, 1.0, 1.0 };
    const GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

    glLightfv( GL_LIGHT0, GL_AMBIENT,  light_ambient );
    glLightfv( GL_LIGHT0, GL_DIFFUSE,  light_diffuse );
    glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular );

    glLightModelf( GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE );

    const GLfloat mat_ambient[]    = { 0.0, 0.3, 0.3, 1.0 };
    const GLfloat mat_diffuse[]    = { 0.0, 0.5, 0.6, 1.0 };
    const GLfloat mat_specular[]   = { 1.0, 1.0, 1.0, 1.0 };
    const GLfloat mat_shininess[]  = { 32.0 };

    glMaterialfv( GL_FRONT, GL_AMBIENT,   mat_ambient );
    glMaterialfv( GL_FRONT, GL_DIFFUSE,   mat_diffuse );
    glMaterialfv( GL_FRONT, GL_SPECULAR,  mat_specular );
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess );

//otras configuraciones como luz, ocu
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );
    glEnable( GL_AUTO_NORMAL );
    glEnable( GL_NORMALIZE );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LESS );
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
    glShadeModel( GL_SMOOTH );

//colores extras para los sólidos
  const GLfloat fog_color[] = { 0.4, 0.9, 1, 1.0 };

    glFogi( GL_FOG_MODE, GL_EXP2 );
    glFogf( GL_FOG_DENSITY, 0.1 );
    glHint( GL_FOG_HINT, GL_NICEST );
    glFogfv( GL_FOG_COLOR, fog_color );

    glClearColor( fog_color[0], fog_color[1], fog_color[2], fog_color[3] );
}

//---------------------------------------------------------------------------
//habilita la ventana que permite mostrar el robot
void cbReshape( int width, int height ){
    float fieldOfViewY = 30.0;
    float near = 2.0;
    float far = 8.0;
    float aspect = (float) width / (float) height;

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( fieldOfViewY, aspect, near, far );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    glViewport( 0, 0, width, height );
}

//----------------------------------------------------------------------------
//restablece los valores de los ángulos
void reinicio(){
	angle[0][0]=0;
	angle[0][2]=0;
	angle[0][4]=0;
	angle[0][6]=0;
	angle[0][1]=0.1;
	angle[0][3]=0.1;
	angle[0][5]=0.1;
	angle[0][7]=0.1;
	angle[1][0]=0;
	angle[1][2]=0;
	angle[1][4]=0;
	angle[1][6]=0;
	angle[1][1]=0.1;
	angle[1][3]=0.1;
	angle[1][5]=0.1;
	angle[1][7]=0.1;
}
//teclado
void cbKeyboard( unsigned char key, int x, int y ){
    switch ( key ){
		case ' ':
			accion=2;break;
		case '1':
	    	reinicio();
			accion=1;break;
		case '2':
			accion=3;break;
		case '3':
			reinicio();
			angle[1][0]=90;
			accion=4;break;
		case '4':
			accion=5;break;
		case '5':
			reinicio();
			angle[1][2]=90;
			accion=6;break;
    }
    glutPostRedisplay();//ejecuta nuevamente el display
}

//----------------------------------------------------------------------------
//mouse
void cbMouseClick( int button, int state, int x, int y ){
    switch ( button )
    {
	case GLUT_LEFT_BUTTON://mientras se mantiene presionado se permite mover la cámara
	    buttonPressed[0] = ( state == GLUT_DOWN );
	    break;
	case GLUT_MIDDLE_BUTTON:
	    buttonPressed[1] = ( state == GLUT_DOWN );
	    break;
	case GLUT_RIGHT_BUTTON:
	    buttonPressed[2] = ( state == GLUT_DOWN );
	    break;
    }
    lastx = x;
    lasty = y;
}
//mouse
void cbMouseMove( int x, int y ){
    const int dx = x - lastx;
    const int dy = y - lasty;
    lastx = x;
    lasty = y;

    if ( buttonPressed[0] )
    {
	rotx += (float) 0.5 * dy;
	roty += (float) 0.5 * dx;
    }
    else if ( buttonPressed[1] )
    {
	zoom -= (float) 0.05 *  dy;
    }
    else if ( buttonPressed[2] )
    {
	tx += (float) 0.01 * dx;
	ty -= (float) 0.01 * dy;
    }
    glutPostRedisplay();
} 
//permite crear el suelo
void base(){
	GLfloat suelo_ambient[]     = { 0.0, 01.0, 0.0, 0.0 };
    GLfloat suelo_diffuse[]     = { 0.0, 01.0, 0.50, 1.0 };
    GLfloat suelo_specular[]    = { 0.0, 01.0, 0.50, 1.0 };
    GLfloat suelo_shininess[]   = { 32.0 };

    glMaterialfv( GL_FRONT, GL_AMBIENT, suelo_ambient );
    glMaterialfv( GL_FRONT, GL_DIFFUSE, suelo_diffuse );
    glMaterialfv( GL_FRONT, GL_SPECULAR, suelo_specular );
    glMaterialfv( GL_FRONT, GL_SHININESS, suelo_shininess );
    
	glPushMatrix();
	glTranslatef(0,-1.54,0);
	glColor3f(0,1,0);
	glScalef(5,0.1,5); 
	glutSolidCube(5);
	glPopMatrix();
}

//movimientos del robot
void acciones(int acto){
	if(accion==1){
		if(acto==0){
			glTranslatef(0,angle[2][4]/90,angle[2][0]/90);
			angle[2][0]+=angle[2][1]; 
			angle[2][4]+=angle[2][5];
			if(angle[2][0]>=180){ 
				angle[2][1]=-0.1;
			}if(angle[2][0]<=-180){ 
				angle[2][1]=0.1;
			}if(angle[2][4]>0.1){
				angle[2][5]=-0.1;
			}if(angle[2][4]<-0.1){
				angle[2][5]=0.1;
			}
		}if(acto==1){
			glRotatef( angle[0][0], -1, 0, 0);
			angle[0][0]+=angle[0][1];
	    	if(angle[0][0]>=45){
	    		angle[0][1]=-0.1; 
			}if(angle[0][0]<=-45){ 
				angle[0][1]=0.1;
			} 
		}if(acto==2){
			glRotatef( angle[0][2], -1, 0, 0);
			angle[0][2]+=angle[0][3];
	    	if(angle[0][2]>=90){
	    		angle[0][3]=-0.1; 
			}if(angle[0][2]<=0){ 
				angle[0][3]=0.1;
			} 
		}if(acto==3){
			glRotatef( angle[0][4], -1, 0, 0);
			angle[0][4]+=-angle[0][5];
	    	if(angle[0][4]>=45){
	    		angle[0][5]=0.1; 
			}if(angle[0][4]<=-45){ 
				angle[0][5]=-0.1;
			}	
		}if(acto==4){
			glRotatef( angle[0][6], -1, 0, 0);
			angle[0][6]+=angle[0][7];
	    	if(angle[0][6]>=90){
	    		angle[0][7]=-0.1; 
			}if(angle[0][6]<=0){ 
				angle[0][7]=0.1;
			}
		}if(acto==5){
			glRotatef( angle[1][0], -1, 0, 0);
			angle[1][0]+=angle[1][1];
	    	if(angle[1][0]>=45){
	    		angle[1][1]=-0.1; 
			}if(angle[1][0]<=-45){ 
				angle[1][1]=0.1;
			} 
		}if(acto==6){
			glRotatef( angle[1][2], 1, 0, 0);
			angle[1][2]+=angle[1][3];
	    	if(angle[1][2]>=40){
	    		angle[1][3]=-0.1; 
			}if(angle[1][2]<=-10){ 
				angle[1][3]=0.1;
			} 
		}if(acto==7){
			glRotatef( angle[1][4], -1, 0, 0);
			angle[1][4]+=-angle[1][5];
	    	if(angle[1][4]>=45){
	    		angle[1][5]=0.1; 
			}if(angle[1][4]<=-45){ 
				angle[1][5]=-0.1;
			}
		}if(acto==8){
			glRotatef( angle[1][6], 1, 0, 0);
			angle[1][6]+=angle[1][7];
	    	if(angle[1][6]>=80){
	    		angle[1][7]=-0.1; 
			}if(angle[1][6]<=-10){ 
				angle[1][7]=0.1;
			}
		}if(acto==9){
			glRotatef( angle[1][2], 1, 0, 0 );
		}
	}
	//mantener posicion()
	if(accion==2){
		if(acto==0){
			glTranslatef(0,angle[2][4]/90,angle[2][0]/90);
		}if(acto==1){
			glRotatef( angle[0][0], -1, 0, 0);
			angle[0][0]+=angle[0][1];
			if(angle[0][0]>0.1){
	    		angle[0][1]=-0.1; 
			}if(angle[0][0]<-0.1){ 
				angle[0][1]=0.1;
			}	
		}if(acto==2){
			glRotatef( angle[0][2], -1, 0, 0);
			angle[0][2]+=angle[0][3];
			if(angle[0][2]>0.1){
	    		angle[0][3]=-0.1; 
			}if(angle[0][2]<-0.1){ 
				angle[0][3]=0.1;
			}
		}if(acto==3){
			glRotatef( angle[0][4], -1, 0, 0);
			angle[0][4]+=angle[0][5];
			if(angle[0][4]>0.1){
	    		angle[0][5]=-0.1; 
			}if(angle[0][4]<-0.1){ 
				angle[0][5]=0.1;
			}
		}if(acto==4){
			glRotatef( angle[0][6], -1, 0, 0);
			angle[0][6]+=angle[0][7];
			if(angle[0][6]>0.1){
	    		angle[0][7]=-0.1; 
			}if(angle[0][6]<-0.1){ 
				angle[0][7]=0.1;
			}
		}if(acto==5){
			glRotatef( angle[1][0], -1, 0, 0);
			angle[1][0]+=angle[1][1];
			if(angle[1][0]>0.1){
	    		angle[1][1]=-0.1; 
			}if(angle[1][0]<-0.1){ 
				angle[1][1]=0.1;
			}
		}if(acto==6){
			glRotatef( angle[1][2], 1, 0, 0);
			angle[1][2]+=angle[1][3];
			if(angle[1][2]>0.1){
	    		angle[1][3]=-0.1; 
			}if(angle[1][2]<-0.1){ 
				angle[1][3]=0.1;
			}
		}if(acto==7){
			glRotatef( angle[1][4], -1, 0, 0);
			angle[1][4]+=angle[1][5];
			if(angle[1][4]>0.1){
	    		angle[1][5]=-0.1; 
			}if(angle[1][4]<-0.1){ 
				angle[1][5]=0.1;
			}
		}if(acto==8){
			glRotatef( angle[1][6], 1, 0, 0);
			angle[1][6]+=angle[1][7];
			if(angle[1][6]>0.1){
	    		angle[1][7]=-0.1; 
			}if(angle[1][6]<-0.1){ 
				angle[1][7]=0.1;
			}
		}if(acto==9){
			glRotatef( angle[1][2], 1, 0, 0 );
		}
	}
	//mantener y regresar (2)
	if(accion==3){
		if(acto==0){
			glTranslatef(0,angle[2][4]/90,angle[2][0]/90);
			angle[2][0]+=angle[2][1];
			angle[2][4]+=angle[2][5];	 
			if(angle[2][0]>0.1){
	    		angle[2][1]=-0.1; 
			}if(angle[2][0]<-0.1){ 
				angle[2][1]=0.1;
			}if(angle[2][4]>0.1){
				angle[2][5]=-0.1;
			}if(angle[2][4]<-0.1){
				angle[2][5]=0.1;
			}
		}if(acto==1){
			glRotatef( angle[0][0], -1, 0, 0);
			angle[0][0]+=angle[0][1];
			if(angle[0][0]>0.1){
	    		angle[0][1]=-0.1; 
			}if(angle[0][0]<-0.1){ 
				angle[0][1]=0.1;
			}	
		}if(acto==2){
			glRotatef( angle[0][2], -1, 0, 0);
			angle[0][2]+=angle[0][3];
			if(angle[0][2]>0.1){
	    		angle[0][3]=-0.1; 
			}if(angle[0][2]<-0.1){ 
				angle[0][3]=0.1;
			}
		}if(acto==3){
			glRotatef( angle[0][4], -1, 0, 0);
			angle[0][4]+=angle[0][5];
			if(angle[0][4]>0.1){
	    		angle[0][5]=-0.1; 
			}if(angle[0][4]<-0.1){ 
				angle[0][5]=0.1;
			}
		}if(acto==4){
			glRotatef( angle[0][6], -1, 0, 0);
			angle[0][6]+=angle[0][7];
			if(angle[0][6]>0.1){
	    		angle[0][7]=-0.1; 
			}if(angle[0][6]<-0.1){ 
				angle[0][7]=0.1;
			}
		}if(acto==5){
			glRotatef( angle[1][0], -1, 0, 0);
			angle[1][0]+=angle[1][1];
			if(angle[1][0]>0.1){
	    		angle[1][1]=-0.1; 
			}if(angle[1][0]<-0.1){ 
				angle[1][1]=0.1;
			}
		}if(acto==6){
			glRotatef( angle[1][2], 1, 0, 0);
			angle[1][2]+=angle[1][3];
			if(angle[1][2]>0.1){
	    		angle[1][3]=-0.1; 
			}if(angle[1][2]<-0.1){ 
				angle[1][3]=0.1;
			}
		}if(acto==7){
			glRotatef( angle[1][4], -1, 0, 0);
			angle[1][4]+=angle[1][5];
			if(angle[1][4]>0.1){
	    		angle[1][5]=-0.1; 
			}if(angle[1][4]<-0.1){ 
				angle[1][5]=0.1;
			}
		}if(acto==8){
			glRotatef( angle[1][6], 1, 0, 0);
			angle[1][6]+=angle[1][7];
			if(angle[1][6]>0.1){
	    		angle[1][7]=-0.1; 
			}if(angle[1][6]<-0.1){ 
				angle[1][7]=0.1;
			}
		}if(acto==9){
			glRotatef( angle[1][2], 1, 0, 0 );
		}
	}
	//alegre(e)
	if(accion==4){
		if(acto==0){
			glTranslatef(0,angle[2][4]/90,angle[2][0]/90);
			glRotatef(angle[2][2], 0, 1, 0);
			angle[2][2]+=angle[2][3];	 
			angle[2][4]+=angle[2][5];
			if(angle[2][2]>=45){ 
				angle[2][3]=-0.1;
			}if(angle[2][2]<=-45){ 
				angle[2][3]=0.1;
			}if(angle[2][4]>0.1){
				angle[2][5]=-0.1;
			}if(angle[2][4]<-0.1){
				angle[2][5]=0.1;
			}
		}if(acto==1){
			glRotatef(angle[0][0], 0, 0, -1);
			angle[0][0]+=angle[0][1];
	    	if(angle[0][0]>=120){
	    		angle[0][1]=-0; 
			}if(angle[0][0]<=0){ 
				angle[0][1]=0.1;
			}
		}if(acto==2){
			glRotatef( angle[0][2], 1, 0, 1);
			angle[0][2]+=angle[0][3];
	    	if(angle[0][2]>=10){
	    		angle[0][3]=-0.1; 
			}if(angle[0][2]<=-90){ 
				angle[0][3]=0.1;
			} 
		}if(acto==3){
			glRotatef( angle[0][4], 0, 0, 1);
			angle[0][4]+=angle[0][5];
	    	if(angle[0][4]>=120){
	    		angle[0][5]=-0.0; 
			}if(angle[0][4]<=0){ 
				angle[0][5]=0.1;
			} 
		}if(acto==4){
			glRotatef( angle[0][6], 1, 0, -1);
			angle[0][6]+=angle[0][7];
	    	if(angle[0][6]>=10){
	    		angle[0][7]=-0.1; 
			}if(angle[0][6]<=-90){ 
				angle[0][7]=0.1;
			} 
		}if(acto==5){
			glRotatef( angle[1][0], -1, 0, 0);
			angle[1][0]+=angle[1][1];
	    	if(angle[1][0]<=0){
	    		angle[1][1]=0.1; 
			}if(angle[1][0]>=90){ 
				angle[1][1]=-0.1;
			} 
		}if(acto==6){
			glRotatef( angle[1][2], 1, 0, 0);
			angle[1][2]+=angle[1][3];
	    	if(angle[1][2]>=80){
	    		angle[1][3]=-0.1; 
			}if(angle[1][2]<=-10){ 
				angle[1][3]=0.1;
			} 
		}if(acto==7){
			glRotatef( angle[1][4], -1, 0, 0);
			angle[1][4]+=angle[1][5];
	    	if(angle[1][4]>=90){
	    		angle[1][5]=-0.1; 
			}if(angle[1][4]<=0 ){ 
				angle[1][5]=0.1;
			} 
		}if(acto==8){
			glRotatef( angle[1][6], 1, 0, 0);
			angle[1][6]+=angle[1][7];
	    	if(angle[1][6]>=80){
	    		angle[1][7]=-0.1; 
			}if(angle[1][6]<=-10){ 
				angle[1][7]=0.1;
			} 
		}if(acto==9){
			glRotatef( angle[1][2]/2, 1, 0, 0 );
		}
	}
	//pasito (4)
	if(accion==5){
		if(acto==0){
			glTranslatef(0,0,angle[2][0]/90);
			glRotatef(angle[2][2], 0, 1, 0);
			angle[2][2]+=angle[2][3];	 
			if(angle[2][2]>=90){ 
				angle[2][3]=-0.1;
			}if(angle[2][2]<=-90){ 
				angle[2][3]=0.1;
			}
		}if(acto==1){
			glRotatef(angle[0][0], 0, 0, -1);
			angle[0][0]+=angle[0][1];
	    	if(angle[0][0]>=90){
	    		angle[0][1]=-0; 
			}if(angle[0][0]<=0){ 
				angle[0][1]=0.1;
			}
		}if(acto==2){
			glRotatef( angle[0][2], 0, 0, 1);
			angle[0][2]+=angle[0][3];
	    	if(angle[0][2]>=120){
	    		angle[0][3]=-0.1; 
			}if(angle[0][2]<=-0){ 
				angle[0][3]=0.1;
			} 
		}if(acto==3){
			glRotatef( angle[0][4], -1, 0, 0);
			angle[0][4]+=angle[0][5];
	    	if(angle[0][4]>=90){
	    		angle[0][5]=-0.0; 
			}if(angle[0][4]<=0){ 
				angle[0][5]=0.1;
			} 
		}if(acto==4){
			glRotatef( angle[0][6], -1, 0, 0);
			angle[0][6]+=angle[0][7];
	    	if(angle[0][6]>=90){
	    		angle[0][7]=-0.1; 
			}if(angle[0][6]<=0){ 
				angle[0][7]=0.1;
			} 
		}if(acto==5){
			glRotatef( angle[1][0], -1, 1, 0);
			angle[1][0]+=angle[1][1];
	    	if(angle[1][0]<=0){
	    		angle[1][1]=0.1; 
			}if(angle[1][0]>=90){ 
				angle[1][1]=-0.1;
			} 
		}if(acto==6){
			glRotatef( angle[1][2], 1, 0, 0);
			angle[1][2]+=angle[1][3];
	    	if(angle[1][2]>=80){
	    		angle[1][3]=-0.1; 
			}if(angle[1][2]<=-10){ 
				angle[1][3]=0.1;
			} 
		}if(acto==7){
			glRotatef( angle[1][4], -1, 1, 0);
			angle[1][4]+=angle[1][5];
	    	if(angle[1][4]>=90){
	    		angle[1][5]=-0.1; 
			}if(angle[1][4]<=0 ){ 
				angle[1][5]=0.1;
			} 
		}if(acto==8){
			glRotatef( angle[1][6], 1, 0, 0);
			angle[1][6]+=angle[1][7];
	    	if(angle[1][6]>=80){
	    		angle[1][7]=-0.1; 
			}if(angle[1][6]<=-10){ 
				angle[1][7]=0.1;
			} 
		}if(acto==9){
			glRotatef( angle[1][2]/2, 1, 0, 0 );
		}
	}
	//moscu (5)
	if(accion==6){
		if(acto==0){
			glTranslatef(0,angle[2][4]/90,angle[2][0]/90);
			glRotatef(angle[2][2], 0, 1, 0);
			angle[2][2]+=angle[2][3];	 
			angle[2][4]-=angle[2][5];	 
			if(angle[2][2]>=10){ 
				angle[2][3]=-0.02;
			}if(angle[2][2]<=-10){ 
				angle[2][3]=0.02;
			}if(angle[2][4]<=-20){
				angle[2][5]=-0.0;
			}if(angle[2][4]>=0){
				angle[2][5]=0.1;
			}
		}if(acto==1){
			glRotatef(angle[0][0], -1, 0, 0);
			angle[0][0]+=angle[0][1];
	    	if(angle[0][0]>=80){
	    		angle[0][1]=-0; 
			}if(angle[0][0]<=0){ 
				angle[0][1]=0.1;
			}
		}if(acto==2){
			glRotatef( angle[0][2], 0, 0, 1);
			angle[0][2]+=angle[0][3];
	    	if(angle[0][2]>=90){
	    		angle[0][3]=-0.0; 
			}if(angle[0][2]<=0){ 
				angle[0][3]=0.1;
			} 
		}if(acto==3){
			glRotatef( angle[0][4], -1, 0, 0);
			angle[0][4]+=angle[0][5];
	    	if(angle[0][4]>=80){
	    		angle[0][5]=-0.0; 
			}if(angle[0][4]<=0){ 
				angle[0][5]=0.1;
			} 
		}if(acto==4){
			glRotatef( angle[0][6], 0, 0, -1);
			angle[0][6]+=angle[0][7];
	    	if(angle[0][6]>=100){
	    		angle[0][7]=-0.0; 
			}if(angle[0][6]<=0){ 
				angle[0][7]=0.1;
			} 
		}if(acto==5){
			glRotatef( angle[1][0], -1, 0, 0);
			angle[1][0]+=angle[1][1];
	    	if(angle[1][0]<=0){
	    		angle[1][1]=1; 
			}if(angle[1][0]>=60){ 
				angle[1][1]=-0.0;
			} 
		}if(acto==6){
			glRotatef( angle[1][2], 1, 0, 0);
			angle[1][2]+=angle[1][3];
	    	if(angle[1][2]>=80){
	    		angle[1][3]=-0.3; 
			}if(angle[1][2]<=-10){ 
				angle[1][3]=0.1;
			} 
		}if(acto==7){
			glRotatef( angle[1][4], -1, 0, 0);
			angle[1][4]+=angle[1][5];
	    	if(angle[1][4]>=60){
	    		angle[1][5]=-0.0; 
			}if(angle[1][4]<=0 ){ 
				angle[1][5]=1;
			} 
		}if(acto==8){
			glRotatef( angle[1][6], 1, 0, 0);
			angle[1][6]+=angle[1][7];
	    	if(angle[1][6]>=80){
	    		angle[1][7]=-0.3; 
			}if(angle[1][6]<=-10){ 
				angle[1][7]=0.1;
			} 
		}if(acto==9){
			glRotatef( angle[1][2]/2, 1, 0, 0 );
		}
	}
}
//colores del robot
void colores(int color){
//	negro
	if(color==1){
		GLfloat ojos_der_ambient[]     = { 0.0, 0.0, 0.0, 1.0  };
	    GLfloat ojos_der_diffuse[]     = { 0.0, 0.0, 0.0, 1.0 };
	    GLfloat ojos_der_specular[]    = { 0.0, 0.0, 0.0, 1.0  };
	    GLfloat ojos_der_shininess[]   = { 32.0 };
	
	    glMaterialfv( GL_FRONT, GL_AMBIENT, ojos_der_ambient );
	    glMaterialfv( GL_FRONT, GL_DIFFUSE, ojos_der_diffuse );
	    glMaterialfv( GL_FRONT, GL_SPECULAR, ojos_der_specular );
	    glMaterialfv( GL_FRONT, GL_SHININESS, ojos_der_shininess );
	}
//	morado
	if(color==2){
		GLfloat lobulo_der_azul_ambient[]     = { 0.4, 0.0, 0.9, 1.0  };
	    GLfloat lobulo_der_azul_diffuse[]     = { 0.4, 0.0, 0.9, 1.0 };
	    GLfloat lobulo_der_azul_specular[]    = { 0.4, 0.0, 0.9, 1.0  };
	    GLfloat lobulo_der_azul_shininess[]   = { 32.0 };
	
	    glMaterialfv( GL_FRONT, GL_AMBIENT, lobulo_der_azul_ambient );
	    glMaterialfv( GL_FRONT, GL_DIFFUSE, lobulo_der_azul_diffuse );
	    glMaterialfv( GL_FRONT, GL_SPECULAR, lobulo_der_azul_specular );
	    glMaterialfv( GL_FRONT, GL_SHININESS, lobulo_der_azul_shininess );
	}
//	verde
	if(color==3){
		GLfloat cabeza_ambient[]     = { 0.5, 0.9, 0.3, 1.0  };
	    GLfloat cabeza_diffuse[]     = { 0.5, 0.9, 0.3, 1.0  };
	    GLfloat cabeza_specular[]    = { 0.5, 0.9, 0.3, 1.0  };
	    GLfloat cabeza_shininess[]   = { 32.0 };
	
	    glMaterialfv( GL_FRONT, GL_AMBIENT, cabeza_ambient );
	    glMaterialfv( GL_FRONT, GL_DIFFUSE, cabeza_diffuse );
	    glMaterialfv( GL_FRONT, GL_SPECULAR, cabeza_specular );
	    glMaterialfv( GL_FRONT, GL_SHININESS, cabeza_shininess );
	}
//	plomo
	if(color==4){
		GLfloat hombro_der_ambient[]     = { 0.5, 0.5, 0.5, 1.0};
	    GLfloat hombro_der_diffuse[]     = { 0.5, 0.5, 0.5, 1.0};
	    GLfloat hombro_der_specular[]    = {0.5, 0.5, 0.5, 1.0};
	    GLfloat hombro_der_shininess[]   = { 8.0 };
	
	    glMaterialfv( GL_FRONT, GL_AMBIENT, hombro_der_ambient );
	    glMaterialfv( GL_FRONT, GL_DIFFUSE, hombro_der_diffuse );
	    glMaterialfv( GL_FRONT, GL_SPECULAR, hombro_der_specular );
	    glMaterialfv( GL_FRONT, GL_SHININESS, hombro_der_shininess );
	}
}
//creacion de las partes del robot
void bot(){
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    base();
glPushMatrix(); 
    glTranslatef(0,0.65,0);//cuerpo entero
	acciones(0);
	glTranslatef(0,-0.65,-0.0);
    //Cabeza 
    glPushMatrix();
	    colores(1);
		glPushMatrix();
	    	glTranslatef( 0.2, 1.0, 0.15 ); 
	    	acciones(9); //movimiento
			glTranslatef( 0, 0, 0.15 ); 
	    	glutSolidTorus( 0.035, 0.1, 20, 50 );//ojo derecho
	    	colores(2);
	    	glutSolidSphere(0.07,12,12);//lobulo derecho
	    glPopMatrix(); 
	    glPushMatrix();
	    	glTranslatef( -0.2, 1.0, 0.15 );
	    	acciones(9); //movimiento
			glTranslatef( 0, 0, 0.15 ); 
			colores(1);
	    	glutSolidTorus( 0.035, 0.1, 20, 50 );//ojo izquierdo
	    	colores(2);
	    	glutSolidSphere(0.08,12,12);//lobulo izquierdo
	    glPopMatrix(); 
	    glPushMatrix();
	    	colores(4);
			glTranslatef( -0.4, 1, 0);  	
	    	glutSolidSphere(0.08,12,12); //oreja izquierda
	    	glTranslatef( -0.03, 0.2, 0); 
			glScalef(1,50,1); 
	    	glutSolidCube(0.01);//antena izquierda
	    glPopMatrix(); 
	    glPushMatrix();
			glTranslatef( 0.4, 1, 0);  	
	    	glutSolidSphere(0.08,12,12); //oreja derecha
	    	glTranslatef( 0.03, 0.2, 0); 
			glScalef(1,50,1); 
	    	glutSolidCube(0.01);//antena derecha
	    glPopMatrix(); 
	    glTranslatef( 0.0, 1.0, 0.0 );
	    glScalef(2,0.9,1.5);
	    acciones(9);
	    colores(3);
	    glutSolidCube(0.4); //cabeza
	glPopMatrix(); 
    //Tronco superior
    glPushMatrix();
	    glPushMatrix();
	    	glScalef(0.75,1,0.5);//piramide delgada
	    	glRotatef(-90,1,0,0); //rotar la piramide 
	    	glPushMatrix();
		    	glRotatef(45,0,0,1);
			    glTranslatef( 0.0, 0.0, 0.0 );
			    colores(3);
			    glutSolidCone(0.8,1.2,4,3);//tronco
		 	glPopMatrix();
	    glPopMatrix(); 
	    //Tronco inferior
	    glPushMatrix();
		    glPushMatrix();
		    	glScalef(0.75,0.25,0.5);//piramide delgada
		    	glRotatef(90,1,0,0); //rotar la piramide 
		    	glPushMatrix();
			    	glRotatef(45,0,0,1);
				    glTranslatef( 0.0, 0.0, 0.0 );
				    colores(3);
				    glutSolidCone(0.8,1.2,4,3);//tronco
		    	glPopMatrix();
	    	glPopMatrix();  
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
		//extremidades superiores
		//brazo izquierdo 
			glPushMatrix();
				glPushMatrix();
					glTranslatef( -0.3, 0.65, 0.0 ); 
					colores(3);
					glutSolidSphere(0.15,12,12);//hombro derecho
				glPopMatrix();
				glTranslatef(-0.3,0.60,0);
				glRotatef(-20,0,0,1);
				acciones(1); //se invoca la función con una entrada igual a 1 
				glTranslatef(0.6,-0.75,0); 
			    glPushMatrix();
				    glTranslatef( -0.6, 0.5, 0.0 ); 
					glScalef(0.15,1.0,0.15);
					colores(4);
				    glutSolidCube(0.5);//brazo
			    glPopMatrix(); 
				glPushMatrix();
				    glTranslatef( -0.6, 0.2, 0.0 ); 
				    colores(4);
					glutSolidSphere(0.1,12,12); //codo izquierdo
			    glPopMatrix(); 
	    //antebrazo izquierdo
			    glPushMatrix();
				    glTranslatef( -0.6, 0.2, 0.0 ); 
					acciones(2);
					glScalef(0.15,1.0,0.15); 
					glTranslatef(0,-0.21,0);
				    glutSolidCube(0.5); //antebrazo 
				    //mano izquierda
					glPushMatrix(); 
					glScalef(6.15,1,5.15);
					glTranslatef(0.6,0.0,-0.0);
					glPushMatrix();
					    glPushMatrix();
					    	glRotatef(-30,0,0,1);
						    glTranslatef( -0.4, -0.6, 0.0 );
							glScalef(0.1,0.7,0.7);
							colores(3);
							glutSolidCube(0.2); 
					    glPopMatrix(); 
					    glPushMatrix();
					    	glRotatef(20,0,0,1);
						    glTranslatef( -0.77, -0.2, 0.0 );
							glScalef(0.1,0.7,0.7);
							colores(3);
							glutSolidCube(0.2); 
					    glPopMatrix(); 
					    glPushMatrix();
					    	glRotatef(45,0,0,1);
						    glTranslatef( -0.60, 0.17, 0.0 );
							glScalef(0.1,0.7,0.7);
							colores(3);
							glutSolidCube(0.2); 
					    glPopMatrix(); 
					    glPushMatrix();
					    	glRotatef(20,0,0,1);
						    glTranslatef( -0.59, -0.22, 0.0 );
							glScalef(0.1,0.7,0.7);
							colores(3);
							glutSolidCube(0.2); 
					    glPopMatrix();
					glPopMatrix();
				glPopMatrix(); 
			glPopMatrix();  
    	glPopMatrix();
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
	    //brazo derecho 
	    glPushMatrix();
	    	glPushMatrix();
				glTranslatef( 0.3, 0.65, 0.0 ); 
				colores(3);
				glutSolidSphere(0.15,12,12);//hombro derecho
			glPopMatrix(); 
			glRotatef(20,0,0,1);
			glTranslatef(0.5,0.5,0);
			acciones(3);	
			glTranslatef(-0.6,-0.75,-0.0); 
		    glPushMatrix();
			    glTranslatef( 0.6, 0.5, 0.0 ); 
				glScalef(0.15,1.0,0.15);
				colores(4);
			    glutSolidCube(0.5);//brazo
		    glPopMatrix();
		    glPushMatrix();
			    glTranslatef( 0.6, 0.2, 0.0 ); 
				glutSolidSphere(0.1,12,12);//codo derecho	
		    glPopMatrix(); 
			//antebrazo derecho
		    glPushMatrix();
			    glTranslatef( 0.6, 0.2, 0.0 ); 
			    acciones(4);		
				glScalef(0.15,1.0,0.15);
				glTranslatef(0,-0.21,0);
			    glutSolidCube(0.5);
			    //mano derecha
				glPushMatrix();
					glScalef(6.15,1,5.15);
					glTranslatef(0.6,0.0,-0.0);
				    glPushMatrix();
				    	glRotatef(-30,0,0,1);
					    glTranslatef( -0.4, -0.6, 0.0 );
						glScalef(0.1,0.7,0.7);
						colores(3);
						glutSolidCube(0.2); 
				    glPopMatrix(); 
				    glPushMatrix();
				    	glRotatef(20,0,0,1);
					    glTranslatef( -0.77, -0.2, 0.0 );
						glScalef(0.1,0.7,0.7);
						colores(3);
						glutSolidCube(0.2); 
				    glPopMatrix(); 
				    glPushMatrix();
				    	glRotatef(45,0,0,1);
					    glTranslatef( -0.60, 0.17, 0.0 );
						glScalef(0.1,0.7,0.7);
						colores(3);
						glutSolidCube(0.2); 
				    glPopMatrix(); 
				    glPushMatrix();
				    	glRotatef(20,0,0,1);
					    glTranslatef( -0.59, -0.22, 0.0 );
						glScalef(0.1,0.7,0.7);
						colores(3);
						glutSolidCube(0.2); 
				    glPopMatrix();
				glPopMatrix(); 
		    glPopMatrix(); 
    	glPopMatrix();
 	glPopMatrix();
////////////////////////////////////////////////////////////////    
////////////////////////////////////////////////////////////////    
////////////////////////////////////////////////////////////////    
    //muslo derecho
    glPushMatrix(); 
		glTranslatef(0,0.0,0.0); 
		acciones(5); 
		glTranslatef(0,-0.0,-0.0); 
	    glPushMatrix();
		    glTranslatef( 0.25, -0.4, 0.0 ); 
			glScalef(0.15,1.5,0.15);
			colores(4);
		    glutSolidCube(0.5);//muslo
	    glPopMatrix(); 
	    glPushMatrix();
		    glScalef(1.05,1.05,1.05);
		    glTranslatef( 0.25, -0.65, 0.0); 
			glutSolidSphere(0.1,12,12); //rodilla derecha
	    glPopMatrix(); 
    	//pierna derecha 
	    glPushMatrix(); 
		    glTranslatef( 0.2, -0.7, 0.0 );
		    acciones(6);   
			glTranslatef(0.05,-0.2, 0.0 );
			glScalef(0.15,1.0,0.15);
		    glutSolidCube(0.5);//pierna
		    //pie derecho  
		    glPushMatrix();
			    glTranslatef( 0.0, -0.3, 0.7 );
			    glScalef(4,0.20,6);
			    acciones(9);
			    colores(3);
			    glutSolidCube(0.4);
		    glPopMatrix();
	    glPopMatrix();
	glPopMatrix();
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
    //pierna izquierda
    glPushMatrix();
		glTranslatef(0,0.0,0.0);
		acciones(7);		
		glTranslatef(0,-0.0,-0.0); 
	    //muslo izquierdo 
	    glPushMatrix();
		    glTranslatef( -0.25, -0.4, 0.0 ); 
			glScalef(0.15,1.5,0.15);
			colores(4);
		    glutSolidCube(0.5);//muslo
	    glPopMatrix(); 
	    glPushMatrix();
		    glScalef(1.05,1.05,1.05);
		    glTranslatef( -0.25, -0.65, 0.0); 
			glutSolidSphere(0.1,12,12); //rodilla izquierda
	    glPopMatrix(); 
	    glPushMatrix();
		    glTranslatef( -0.2, -0.7, 0.0 ); 
		    acciones(8);  
			glTranslatef(-0.05, -0.2, 0.0 ); 
			glScalef(0.15,1.0,0.15);
		    glutSolidCube(0.5);//pierna
	    	//pie izquierdo 
		    glPushMatrix();
			    glTranslatef( 0.0, -0.3, 0.7 );
			    glScalef(4,0.20,6);
			    acciones(9);
			    colores(3);
			    glutSolidCube(0.4);
		    glPopMatrix();
	    glPopMatrix();
	glPopMatrix();
glPopMatrix();
	glutSwapBuffers();
}

//---------------------------------------------------------------------------

void cbDrawPicture( void ){
    glLoadIdentity();
    //camara
    gluLookAt( 0.0, 2.0, 5.0, // eye coordinates
	       0.0, 0.0, 0.0, // look at point
	       0.0, 1.0, 0.0 ); // up direction 
    //------------------------------
    glLoadIdentity();
    gluLookAt( 0.0, 0.0, 5,   // eye position
	       0.0, 0, 0,   // look-at point
	       0.0, 1.0, 0 ); // arriba 
    // position light in the scene 
	GLfloat light_position[] = { 2.0, 3.0, 0.0, 1.0 };
    glLightfv( GL_LIGHT0, GL_POSITION, light_position ); 
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 
    // draw object 
    glTranslatef( 0.0, 0.0, -zoom ); //boton medio
    glTranslatef( tx, ty, 0.0 ); //clic derecho
    glRotatef( rotx, 1.0, 0.0, 0.0 ); //clic izquierdo eje x
    glRotatef( roty, 0.0, 1.0, 0.0 ); //clic izquierdo eje y
    bot(); //ejecuta el robot 
    glFlush();
    glutSwapBuffers();
    glFlush();
    glutSwapBuffers();
}
//menu de opciones
void menu(void){
	cout<<"\tProyecto Robot Colon"<<endl;
	cout<<"\nPresione:"<<endl;
	cout<<"\n[ ]: Mantener posicion"<<endl;
	cout<<"\n[1]: Caminar"<<endl;	
	cout<<"\n[2]: Mantener posicion y regresar al origen"<<endl;
	cout<<"\n[3]: Baile Alegre"<<endl;
	cout<<"\n[4]: Baile Moderno"<<endl;
	cout<<"\n[5]: Baile Moscu"<<endl;
	
}

//funcion principal
int main( int argc, char *argv[] )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE );
    glutInitWindowSize( 400, 400 );
    glutInitWindowPosition( 0, 0 );
    glutCreateWindow("Robot de Colón");
    glutReshapeFunc( cbReshape );
    glutKeyboardFunc( cbKeyboard );
    glutMouseFunc( cbMouseClick ); 
    glutMotionFunc( cbMouseMove );
    glutDisplayFunc(cbDrawPicture);
	glutIdleFunc(cbDrawPicture);
    graphicsInit();
    
	menu();
    glutMainLoop();

    return 0;
}

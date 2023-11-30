// **********************************************************************
// PUCRS/Escola Politécnica
// COMPUTAÇÃO GRÁFICA
//
// Programa básico para criar aplicacoes 3D em OpenGL
//
// Marcio Sarroglia Pinho
// pinho@pucrs.br
// **********************************************************************

#include <iostream>
#include <cmath>
#include <ctime>

using namespace std;

#ifdef WIN32
#include <windows.h>
#include "gl\glut.h"
    static DWORD last_idle_time;
#else
    #include <sys/time.h>
    static struct timeval last_idle_time;
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif


#include "SOIL/SOIL.h"

#include "TextureClass.h"

#include "ListaDeCoresRGB.h"

#include "Ponto.h"

GLfloat AspectRatio, AngY=0;
GLuint TEX1, TEX2;
bool ComTextura = true;

Ponto CantoEsquerdo = Ponto (-20,-1,-10);
// **********************************************************************
//  void initTexture(void)
//        Define a textura a ser usada
//
// **********************************************************************
void initTexture (void)
{
    TEX1 = LoadTexture ("Falcao.jpg");
    TEX2 = LoadTexture ("PressioneT.png");
}

// **********************************************************************
//  void DefineLuz(void)
// **********************************************************************
void DefineLuz(void)
{
  // Define cores para um objeto dourado
  GLfloat LuzAmbiente[]   = {0.4, 0.4, 0.4 } ;
  GLfloat LuzDifusa[]   = {0.7, 0.7, 0.7};
  GLfloat LuzEspecular[] = {0.9f, 0.9f, 0.9 };
  GLfloat PosicaoLuz0[]  = {0.0f, 3.0f, 5.0f };  // Posição da Luz
  GLfloat Especularidade[] = {1.0f, 1.0f, 1.0f};

   // ****************  Fonte de Luz 0

 glEnable ( GL_COLOR_MATERIAL );

   // Habilita o uso de iluminação
  glEnable(GL_LIGHTING);

  // Ativa o uso da luz ambiente
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LuzAmbiente);
  // Define os parametros da luz número Zero
  glLightfv(GL_LIGHT0, GL_AMBIENT, LuzAmbiente);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, LuzDifusa  );
  glLightfv(GL_LIGHT0, GL_SPECULAR, LuzEspecular  );
  glLightfv(GL_LIGHT0, GL_POSITION, PosicaoLuz0 );
  glEnable(GL_LIGHT0);

  // Ativa o "Color Tracking"
  glEnable(GL_COLOR_MATERIAL);

  // Define a reflectancia do material
  glMaterialfv(GL_FRONT,GL_SPECULAR, Especularidade);

  // Define a concentraçãoo do brilho.
  // Quanto maior o valor do Segundo parametro, mais
  // concentrado será o brilho. (Valores válidos: de 0 a 128)
  glMateriali(GL_FRONT,GL_SHININESS,51);

}


// **********************************************************************
//  void init(void)
//		Inicializa os par‚metros globais de OpenGL
//
// **********************************************************************
void init(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Fundo de tela preto

	glShadeModel(GL_SMOOTH);
    //glShadeModel(GL_FLAT);
	glColorMaterial ( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );

	glEnable(GL_DEPTH_TEST);
	//glEnable ( GL_CULL_FACE );
    glDisable ( GL_CULL_FACE );

    glEnable(GL_NORMALIZE);
    initTexture();
	
}
// **********************************************************************
//  void PosicUser()
// **********************************************************************
void PosicUser()
{
	// Set the clipping volume
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90,AspectRatio,0.01,200);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    gluLookAt(0, 0, 10,   // Posição do Observador
              0,0,0,     // Posição do Alvo
              0.0f,1.0f,0.0f);


}
// **********************************************************************
//  void reshape( int w, int h )
//		trata o redimensionamento da janela OpenGL
//
// **********************************************************************
void reshape( int w, int h )
{

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;

	AspectRatio = 1.0f * w / h;
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);
    //cout << "Largura" << w << endl;

	PosicUser();

}
// **********************************************************************
//   void DesenhaCubo (GLuint nro_da_textura)
//
//
// **********************************************************************
void DesenhaCubo ()
{
    glBegin ( GL_QUADS );
    
    // Front Face
    glNormal3f(0,0,1);
    glTexCoord2f(0.5f, 0.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    
    glTexCoord2f(1.0f, 0.5f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    
    glTexCoord2f(0.5f, 0.5);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    
    // Back Face
    glNormal3f(0,0,-1);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    
    // Top Face
    glNormal3f(0,1,0);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    
    // Bottom Face
    glNormal3f(0,-1,0);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    
    // Right face
    glNormal3f(1,0,0);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    
    // Left Face
    glNormal3f(-1,0,0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glEnd();
    
}
// **********************************************************************
// void DesenhaLadrilho(int idTextura)
// Desenha uma célula do piso.
// Eh possivel definir a cor da borda e do interior do piso
// O ladrilho tem largula 1, centro no (0,0,0) e está sobre o plano XZ
// **********************************************************************
void DesenhaQuadrado(int idTextura)
{
    
    glBegin ( GL_QUADS );
        glNormal3f(0,1,0);
        glVertex3f(-0.5f,  0.0f, -0.5f);
        glVertex3f(-0.5f,  0.0f,  0.5f);
        glVertex3f( 0.5f,  0.0f,  0.5f);
        glVertex3f( 0.5f,  0.0f, -0.5f);
    glEnd();
}
// **********************************************************************
// void DesenhaLadrilho(int corBorda, int corDentro)
// Desenha uma célula do piso.
// Eh possivel definir a cor da borda e do interior do piso
// O ladrilho tem largula 1, centro no (0,0,0) e está sobre o plano XZ
// **********************************************************************
void DesenhaQuadrado(int corBorda, int corDentro)
{

    defineCor(corBorda); // desenha QUAD preenchido
    glBegin ( GL_QUADS );
        glNormal3f(0,1,0);
        glVertex3f(-0.5f,  0.0f, -0.5f);
        glVertex3f(-0.5f,  0.0f,  0.5f);
        glVertex3f( 0.5f,  0.0f,  0.5f);
        glVertex3f( 0.5f,  0.0f, -0.5f);
    glEnd();
    
    defineCor(corDentro);
    glBegin ( GL_LINE_STRIP );
        glNormal3f(0,1,0);
        glVertex3f(-0.5f,  0.0f, -0.5f);
        glVertex3f(-0.5f,  0.0f,  0.5f);
        glVertex3f( 0.5f,  0.0f,  0.5f);
        glVertex3f( 0.5f,  0.0f, -0.5f);
    glEnd();

}
// **********************************************************************
// void DesenhaLadrilho(int corBorda, int corDentro)
// Desenha uma célula do piso.
// Eh possivel definir a cor da borda e do interior do piso
// O ladrilho tem largula 1, centro no (0,0,0) e está sobre o plano XZ
// **********************************************************************
void DesenhaLadrilho(int corBorda, int corDentro)
{
    //defineCor(corDentro);// desenha QUAD preenchido
    glColor3f(1,1,1);
    glBegin ( GL_QUADS );
        glNormal3f(0,1,0);
        glVertex3f(-0.5f,  0.0f, -0.5f);
        glVertex3f(-0.5f,  0.0f,  0.5f);
        glVertex3f( 0.5f,  0.0f,  0.5f);
        glVertex3f( 0.5f,  0.0f, -0.5f);
    glEnd();
    
    //defineCor(corBorda);
    glColor3f(0,1,0);

    glBegin ( GL_LINE_STRIP );
        glNormal3f(0,1,0);
        glVertex3f(-0.5f,  0.0f, -0.5f);
        glVertex3f(-0.5f,  0.0f,  0.5f);
        glVertex3f( 0.5f,  0.0f,  0.5f);
        glVertex3f( 0.5f,  0.0f, -0.5f);
    glEnd();
    

}

// **********************************************************************\
//
// **********************************************************************
void DesenhaPiso()
{
    srand(100); // usa uma semente fixa para gerar sempre as mesma cores no piso
    glPushMatrix();
    glTranslated(CantoEsquerdo.x, CantoEsquerdo.y, CantoEsquerdo.z);
    for(int x=-20; x<20;x++)
    {
        glPushMatrix();
        for(int z=-20; z<20;z++)
        {
            DesenhaLadrilho(White, rand()%40);
            glTranslated(0, 0, 1);
        }
        glPopMatrix();
        glTranslated(1, 0, 0);
    }
    glPopMatrix();
}
// **********************************************************************
//  void display( void )
//
//
// **********************************************************************
void display( void )
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    DefineLuz();
	PosicUser();

	glMatrixMode(GL_MODELVIEW);
    
    glDisable(GL_TEXTURE_2D);
    DesenhaPiso();
    
    if (ComTextura)
        glEnable(GL_TEXTURE_2D);
    else glDisable(GL_TEXTURE_2D);
 
    if (glIsEnabled(GL_TEXTURE_2D))
        glColor3f(1,1,1);
    else glColor3f(0.5, 0, 0);
    
    if (glIsEnabled(GL_TEXTURE_2D))
        glColor3f(1,1,1);
    else glColor3f(0.5f,0.0f,0.0f); // Vermelho
    
	glPushMatrix();
        glTranslatef ( 5.0f, 0.0f, 3.0f );
        glRotatef(AngY,0,1,0);
        glBindTexture (GL_TEXTURE_2D, TEX1);
		DesenhaCubo();
	glPopMatrix();

    if (glIsEnabled(GL_TEXTURE_2D))
        glColor3f(1,1,1);
    else glColor3f(0.6156862745, 0.8980392157, 0.9803921569); // Azul claro
    
	glPushMatrix();
        glTranslatef ( -4.0f, 0.0f, 2.0f );
		//glTranslatef ( 2.0f, 0f, 0.0f );
		glRotatef(AngY,0,1,0);
        glBindTexture (GL_TEXTURE_2D, TEX2);
		DesenhaCubo();
	glPopMatrix();

    
    
    
	glutSwapBuffers();
}

// **********************************************************************
//  void animate ( unsigned char key, int x, int y )
//
//
// **********************************************************************
void animate()
{
    // Anima cubos
    AngY++;

    // Redesenha
    glutPostRedisplay();
}

// **********************************************************************
//  void keyboard ( unsigned char key, int x, int y )
//
//
// **********************************************************************
void keyboard ( unsigned char key, int x, int y )  
{
	switch ( key ) 
	{
    case 27:        // Termina o programa qdo
      exit ( 0 );   // a tecla ESC for pressionada
      break;        
    case 't':
        ComTextura = !ComTextura;
        break;
    default:
            cout << key;
      break;
  }
}

// **********************************************************************
//  void arrow_keys ( int a_keys, int x, int y )  
//
//
// **********************************************************************
void arrow_keys ( int a_keys, int x, int y )  
{
	switch ( a_keys ) 
	{
		case GLUT_KEY_UP:       // When Up Arrow Is Pressed...
			glutFullScreen ( ); // Go Into Full Screen Mode
			break;
	    case GLUT_KEY_DOWN:     // When Down Arrow Is Pressed...
			glutInitWindowSize  ( 700, 500 ); 
			break;
		default:
			break;
	}
}

// **********************************************************************
//  void main ( int argc, char** argv )
//
//
// **********************************************************************
int main ( int argc, char** argv )   
{
	glutInit            ( &argc, argv ); 
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
	glutInitWindowPosition (0,0);
	glutInitWindowSize  ( 700, 700 ); 
	glutCreateWindow    ( "Computacao Grafica - Exemplo com Textura 3D" );
		
	init ();
    //system("pwd");
	
	glutDisplayFunc ( display );  
	glutReshapeFunc ( reshape );
	glutKeyboardFunc ( keyboard );
	glutSpecialFunc ( arrow_keys );
	glutIdleFunc ( animate );

	glutMainLoop ( );          
	return 0; 
}




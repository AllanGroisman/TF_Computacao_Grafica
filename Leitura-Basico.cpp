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
#include <fstream>


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

GLfloat AspectRatio, AngY=0;

// *********************************************************************
//   ESTRUTURAS A SEREM USADAS PARA ARMAZENAR UM OBJETO 3D
// *********************************************************************

typedef struct  // Struct para armazenar um ponto
{
    float X,Y,Z;
    void Set(float x, float y, float z)
    {
        X = x;
        Y = y;
        Z = z;
    }
    void Imprime()
    {
        cout << "X: " << X << " Y: " << Y << " Z: " << Z;
    }
} TPoint;


typedef struct // Struct para armazenar um tri‚ngulo
{
    TPoint P1, P2, P3;
    void imprime()
    {
        cout << "P1 ";  P1.Imprime(); cout << endl;
        cout << "P2 ";  P2.Imprime(); cout << endl;
        cout << "P3 ";  P3.Imprime(); cout << endl;
    }
} TTriangle;

// Classe para armazenar um objeto 3D
class Objeto3D
{
    TTriangle *faces; // vetor de faces
    unsigned int nFaces; // Variavel que armazena o numero de faces do objeto
public:
    Objeto3D()
    {
        nFaces = 0;
        faces = NULL;
    }
    unsigned int getNFaces()
    {
        return nFaces;
    }
    void LeObjeto (char *Nome); // implementado fora da classe
    void ExibeObjeto(); // implementado fora da classe
};


Objeto3D *MundoVirtual;

// *********************************************************************
//   DESCRICAO DO FORMATO ".TRI"
// *********************************************************************

/*
 A primeira linha do arquivo contÈm o n˙mero de tri‚ngulos que forma o objeto
 As demais linhas descrevemas os tri‚ngulos:
 x1 y1 z1 x2 y2 z2 x3 y3 z3
 onde
 x1 y1 z1 : primeiro vÈrtice do tri‚ngulo
 x2 y2 z2 : segundo vÈrtice do tri‚ngulo
 x3 y3 z3 : terceiro vÈrtice do tri‚ngulo
 Um exemplo com dois tri‚ngulos:
 2
 0 0 10 10 0 10 5 10 10
 10 0 10 20 0 10 15 10 10
 
 */
// **********************************************************************
// void LeObjeto(char *Nome)
// **********************************************************************
void Objeto3D::LeObjeto (char *Nome)
{
    // ***************
    // Exercicio
    //      complete esta rotina fazendo a leitura do objeto
    // ***************
    
    ifstream arq;
    arq.open(Nome, ios::in);
    if (!arq)
    {
        cout << "Erro na abertura do arquivo " << Nome << "." << endl;
        exit(1);
    }
    arq >> nFaces;
    faces = new TTriangle[nFaces];
    float x,y,z;
    for (int i=0;i<nFaces;i++)
    {
        // Le os três vértices
        arq >> x >> y >> z; // Vertice 1
        faces[i].P1.Set(x,y,z);
        arq >> x >> y >> z; // Vertice 2
        faces[i].P2.Set(x,y,z);
        arq >> x >> y >> z; // Vertice 3
        faces[i].P3.Set(x,y,z);
        cout << i << ": ";
        faces[i].imprime();
        // Falta ler o RGB da face....
    }
}

// **********************************************************************
// void ExibeObjeto (TTriangle **Objeto)
// **********************************************************************
void Objeto3D::ExibeObjeto ()
{
    // ***************
    // Exercicio
    //      complete esta rotina fazendo a exibicao do objeto
    // ***************
    
    
}

// **********************************************************************
//  void DefineLuz(void)
//
//
// **********************************************************************
void DefineLuz(void)
{
  // Define cores para um objeto dourado
  GLfloat LuzAmbiente[]   = {0.4, 0.4, 0.4f } ;
  GLfloat LuzDifusa[]   = {0.7, 0.7, 0.7};
  GLfloat LuzEspecular[] = {0.9f, 0.9f, 0.9 };
  GLfloat PosicaoLuz0[]  = {3.0f, 3.0f, 0.0f };
  GLfloat Especularidade[] = {1.0f, 1.0f, 1.0f};

   // ****************  Fonte de Luz 0

 glEnable ( GL_COLOR_MATERIAL );

   // Habilita o uso de iluminaÁ„o
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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Fundo de tela preto

	glShadeModel(GL_SMOOTH);
	glColorMaterial ( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
	glEnable(GL_DEPTH_TEST);
	glEnable ( GL_CULL_FACE );
    
    // Obtem o tempo inicial
#ifdef WIN32
    last_idle_time = GetTickCount();
#else
    gettimeofday (&last_idle_time, NULL);
#endif
	
}
// **********************************************************************
//  void PosicUser()
//
//
// **********************************************************************
void PosicUser()
{
	// Set the clipping volume
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90,AspectRatio,0.01,200);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 0,
		      0,0,-8,
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
//  void DesenhaCubo()
//
//
// **********************************************************************
void DesenhaCubo()
{
	glBegin ( GL_QUADS );
		// Front Face
		glNormal3f(0,0,1);
		glVertex3f(-1.0f, -1.0f,  1.0f);
		glVertex3f( 1.0f, -1.0f,  1.0f);
		glVertex3f( 1.0f,  1.0f,  1.0f);
		glVertex3f(-1.0f,  1.0f,  1.0f);
		// Back Face
		glNormal3f(0,0,-1);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f,  1.0f, -1.0f);
		glVertex3f( 1.0f,  1.0f, -1.0f);
		glVertex3f( 1.0f, -1.0f, -1.0f);
		// Top Face
		glNormal3f(0,1,0);
		glVertex3f(-1.0f,  1.0f, -1.0f);
		glVertex3f(-1.0f,  1.0f,  1.0f);
		glVertex3f( 1.0f,  1.0f,  1.0f);
		glVertex3f( 1.0f,  1.0f, -1.0f);
		// Bottom Face
		glNormal3f(0,-1,0);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f( 1.0f, -1.0f, -1.0f);
		glVertex3f( 1.0f, -1.0f,  1.0f);
		glVertex3f(-1.0f, -1.0f,  1.0f);
		// Right face
		glNormal3f(1,0,0);
		glVertex3f( 1.0f, -1.0f, -1.0f);
		glVertex3f( 1.0f,  1.0f, -1.0f);
		glVertex3f( 1.0f,  1.0f,  1.0f);
		glVertex3f( 1.0f, -1.0f,  1.0f);
		// Left Face
		glNormal3f(-1,0,0);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f,  1.0f);
		glVertex3f(-1.0f,  1.0f,  1.0f);
		glVertex3f(-1.0f,  1.0f, -1.0f);
	glEnd();
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

	glPushMatrix();
		glTranslatef ( 2.0f, 0.0f, -5.0f );
        glRotatef(AngY,0,1,0);
		glColor3f(0.5f,0.0f,0.0f); // Vermelho
		DesenhaCubo();
	glPopMatrix();

	glPushMatrix();
		glTranslatef ( -1.0f, 2.0f, -8.0f );
		glRotatef(AngY,0,1,0);
		glColor3f(0.0f,0.6f,0.0f); // Verde
		DesenhaCubo();
	glPopMatrix();
    
    // Exibicao do objeto lido de arquivo
    glPushMatrix();
        glTranslatef ( -1.0f, 2.0f, -8.0f );
        glRotatef(65,0,0,1);
        glRotatef(AngY,1,0,0);
        glColor3f(1.0f,0.3f,0.0f);
        MundoVirtual[0].ExibeObjeto();
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
    static float dt;
    static float AccumTime=0;
    
#ifdef _WIN32
    DWORD time_now;
    time_now = GetTickCount();
    dt = (float) (time_now - last_idle_time) / 1000.0;
#else
    // Figure out time elapsed since last call to idle function
    struct timeval time_now;
    gettimeofday(&time_now, NULL);
    dt = (float)(time_now.tv_sec  - last_idle_time.tv_sec) +
    1.0e-6*(time_now.tv_usec - last_idle_time.tv_usec);
#endif
    AccumTime +=dt;
    if (AccumTime >=3) // imprime o FPS a cada 3 segundos
    {
        cout << 1.0/dt << " FPS"<< endl;
        AccumTime = 0;
    }
    //cout << "AccumTime: " << AccumTime << endl;
    // Anima cubos
    AngY++;
    // Sa;va o tempo para o próximo ciclo de rendering
    last_idle_time = time_now;

        //if  (GetAsyncKeyState(32) & 0x8000) != 0)
          //  cout << "Espaco Pressionado" << endl;
    
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
	glutInitWindowSize  ( 700, 500 ); 
	glutCreateWindow    ( "Computacao Grafica - Exemplo Basico 3D" ); 
		
	init ();
    //system("pwd");
	
	glutDisplayFunc ( display );  
	glutReshapeFunc ( reshape );
	glutKeyboardFunc ( keyboard );
	glutSpecialFunc ( arrow_keys );
	glutIdleFunc ( animate );
    
    // Le o obejto do arquivo
    char Nome[] = "Vaca.tri";
    
    
    // aloca memória para 5 objetos
    MundoVirtual = new Objeto3D[5];
    // carrega o objeto 0
    MundoVirtual[0].LeObjeto (Nome);

	glutMainLoop ( );          
	return 0; 
}




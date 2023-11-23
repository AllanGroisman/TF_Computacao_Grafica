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
#include <glut.h>
#else
#include <sys/time.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <glut.h>
#endif

#include "Temporizador.h"
#include "ListaDeCoresRGB.h"
#include "Ponto.h"
Temporizador T;
double AccumDeltaT=0;


GLfloat AspectRatio, angulo=0;

// Controle do modo de projecao
// 0: Projecao Paralela Ortografica; 1: Projecao Perspectiva
// A funcao "PosicUser" utiliza esta variavel. O valor dela eh alterado
// pela tecla 'p'
int ModoDeProjecao = 1;

// Controle do modo de projecao
// 0: Wireframe; 1: Faces preenchidas
// A funcao "Init" utiliza esta variavel. O valor dela eh alterado
// pela tecla 'e'
int ModoDeExibicao = 1;

//Matrizes e tamanhos
// Parede
int xParede = 15;
int zParede = 25;
boolean matrizParede[15][25];
//Piso
int xPiso = 50;
int zPiso = 25;
boolean matrizPiso[50][25];

//Posicao do OBS
Ponto posOBS = Ponto(0,5,13);
//Posica do alvo
Ponto posAlvo = Ponto(25,5,13);

//Variaveis do VeÌculo
Ponto posVeiculo = Ponto(5,0,13);
Ponto DirecaoCanhao = Ponto(1,0,0);
double rotacaoCanhao;
double rotacaoVeiculo;

//Variaveis do projetil
Ponto posProjetil;
double forca = 5;
boolean atirar = false;



//Andar e voltar
boolean andar = false;
boolean voltar = false;
//3 pessoa
boolean pessoa3 = false;


double nFrames=0;
double TempoTotal=0;
Ponto CantoEsquerdo = Ponto(-20,-1,-10);

// **********************************************************************
//  void init(void)
//        Inicializa os parametros globais de OpenGL
// **********************************************************************
void init(void)
{
    glClearColor(1.0f, 1.0f, 0.0f, 1.0f); // Fundo de tela preto

    glClearDepth(1.0);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glEnable (GL_CULL_FACE );
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
    //glShadeModel(GL_FLAT);

    glColorMaterial ( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
    if (ModoDeExibicao) // Faces Preenchidas??
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    for (int i = 0; i < xParede; i++) {
        for (int j = 0; j < zParede; j++) {
            matrizParede[i][j] = true;
        }
    }
    for (int i = 0; i < xPiso; i++) {
        for (int j = 0; j < zPiso; j++) {
            matrizParede[i][j] = true;
        }
    }


}
// **********************************************************************
//
// **********************************************************************
void animate()
{
    double dt;
    dt = T.getDeltaT();
    AccumDeltaT += dt;
    TempoTotal += dt;
    nFrames++;

    if (AccumDeltaT > 1.0/30) // fixa a atualização da tela em 30
    {
        AccumDeltaT = 0;
        angulo+= 1;
        glutPostRedisplay();
    }
    if (TempoTotal > 5.0)
    {
        cout << "Tempo Acumulado: "  << TempoTotal << " segundos. " ;
        cout << "Nros de Frames sem desenho: " << nFrames << endl;
        cout << "FPS(sem desenho): " << nFrames/TempoTotal << endl;
        TempoTotal = 0;
        nFrames = 0;
    }
}


// **********************************************************************
//  void DesenhaCubo()
// **********************************************************************
void DesenhaCubo(float tamAresta)
{
    glBegin ( GL_QUADS );
    // Front Face
    glNormal3f(0,0,1);
    glVertex3f(-tamAresta/2, -tamAresta/2,  tamAresta/2);
    glVertex3f( tamAresta/2, -tamAresta/2,  tamAresta/2);
    glVertex3f( tamAresta/2,  tamAresta/2,  tamAresta/2);
    glVertex3f(-tamAresta/2,  tamAresta/2,  tamAresta/2);
    // Back Face
    glNormal3f(0,0,-1);
    glVertex3f(-tamAresta/2, -tamAresta/2, -tamAresta/2);
    glVertex3f(-tamAresta/2,  tamAresta/2, -tamAresta/2);
    glVertex3f( tamAresta/2,  tamAresta/2, -tamAresta/2);
    glVertex3f( tamAresta/2, -tamAresta/2, -tamAresta/2);
    // Top Face
    glNormal3f(0,1,0);
    glVertex3f(-tamAresta/2,  tamAresta/2, -tamAresta/2);
    glVertex3f(-tamAresta/2,  tamAresta/2,  tamAresta/2);
    glVertex3f( tamAresta/2,  tamAresta/2,  tamAresta/2);
    glVertex3f( tamAresta/2,  tamAresta/2, -tamAresta/2);
    // Bottom Face
    glNormal3f(0,-1,0);
    glVertex3f(-tamAresta/2, -tamAresta/2, -tamAresta/2);
    glVertex3f( tamAresta/2, -tamAresta/2, -tamAresta/2);
    glVertex3f( tamAresta/2, -tamAresta/2,  tamAresta/2);
    glVertex3f(-tamAresta/2, -tamAresta/2,  tamAresta/2);
    // Right face
    glNormal3f(1,0,0);
    glVertex3f( tamAresta/2, -tamAresta/2, -tamAresta/2);
    glVertex3f( tamAresta/2,  tamAresta/2, -tamAresta/2);
    glVertex3f( tamAresta/2,  tamAresta/2,  tamAresta/2);
    glVertex3f( tamAresta/2, -tamAresta/2,  tamAresta/2);
    // Left Face
    glNormal3f(-1,0,0);
    glVertex3f(-tamAresta/2, -tamAresta/2, -tamAresta/2);
    glVertex3f(-tamAresta/2, -tamAresta/2,  tamAresta/2);
    glVertex3f(-tamAresta/2,  tamAresta/2,  tamAresta/2);
    glVertex3f(-tamAresta/2,  tamAresta/2, -tamAresta/2);
    glEnd();

}
void DesenhaParalelepipedo()
{
    glPushMatrix();
        glTranslatef(5,1,1);
        glScalef(2,1,2);
        glutSolidCube(1);
        //DesenhaCubo(1);
    glPopMatrix();
}
void DesenhaCanhao()
{
    glPushMatrix();
    //VAI PARA O POSICIONAMENTO DO VEICULO E VIRA PARA ONDE ESTA APONTANDO- universal
    glTranslatef(posVeiculo.x,0,posVeiculo.z);
    glRotatef(rotacaoVeiculo,0,1,0);

    ///// BASE DO VEICULO
    //desenha
    glPushMatrix();
    //Desenha a base
    glColor3f(100,0,255);
    glScalef(3,1,2);
    glutSolidCube(1);
    glPopMatrix();
    /////

    //CANHAO
    glPushMatrix();
    //Acha o Cano em relacao ao canhao
    glTranslatef(1,0.75,0);
    //Aplica a rotacao de canhao necessaria
    glRotatef(rotacaoCanhao,0,0,1);
    //desenha o cano
    glColor3f(0,100,255);
    glScalef(1,0.5,0.5);
    glutSolidCube(1);
    glPopMatrix();
    /////

    //PROJETIL
    glPushMatrix();
    //vai na posicao atual do projetil
    glTranslatef(1,0.75,0);
    //atualiza a posicao do projetil caso queira atirar
    posProjetil = posVeiculo + Ponto (1,0.75,0);

    //caso atire, chama funcao que atualiza o posicionamento final do tiro

    //desenha o projetil
    glColor3f(255, 0, 0);
    glutSolidSphere(0.2, 20, 20);

    glPopMatrix();
    //FIM DO VEICULO
    glPopMatrix();

}

void Tiro()
{
    //atirar = false;
    //forca do tiro
    //forca = 5;
    //posicao inicial do projetil
    //posProjetil;
    //Angulo
    //DirecaoCanhao;
    //calcula o vetor de saida
    Ponto vetorSaida = Ponto (posProjetil.x,posProjetil.y,posProjetil.z);
    vetorSaida.soma(DirecaoCanhao.x*forca,0,DirecaoCanhao.z*forca);
    //calcula a distancia entre o ponto inicial e o final
    double distancia = 2*forca*cos(rotacaoCanhao*3.14-180);
    std::cout << "Distancia: " <<distancia << std::endl;

    //Linha

    glPushMatrix();
        glColor3f(255, 0, 0);
        //desenha na origem
        glTranslatef(posProjetil.x,posProjetil.y,posProjetil.z);
        glRotated(rotacaoVeiculo,0,1,0);
        glRotated(rotacaoCanhao,0,0,1);
        glutSolidTeapot(1);
        //glutSolidSphere(0.7, 10, 20);
        //desenha na altura maxima
        glPushMatrix();

        glPopMatrix();


        //desenha no destino
        glTranslatef(-distancia,0,0);
        glutSolidTeapot(1);
        //glutSolidSphere(0.7, 20, 20);
    glPopMatrix();
}
void TestarColisao()
{
    matrizParede[0][1] = false;
    //int um = 10;
    //int dois = 11;
    for (int i = 9; i < 12; i++) {
        for (int j = 10; j < 13; j++) {
            matrizParede[i][j] = false;
        }
    }
}
// **********************************************************************
// void DesenhaLadrilho(int corBorda, int corDentro)
// Desenha uma célula do piso.
// Eh possivel definir a cor da borda e do interior do piso
// O ladrilho tem largula 1, centro no (0,0,0) e está sobre o plano XZ
// **********************************************************************
void DesenhaLadrilho(int corBorda, int corDentro)
{
    defineCor(corDentro);// desenha QUAD preenchido
    //glColor3f(1,1,1);
    glBegin ( GL_QUADS );
        glNormal3f(0,1,0);
        glVertex3f(-0.5f,  0.0f, -0.5f);
        glVertex3f(-0.5f,  0.0f,  0.5f);
        glVertex3f( 0.5f,  0.0f,  0.5f);
        glVertex3f( 0.5f,  0.0f, -0.5f);
    glEnd();

    defineCor(corBorda);
    glColor3f(0,1,0);

    glBegin ( GL_LINE_STRIP );
        glNormal3f(0,1,0);
        glVertex3f(-0.5f,  0.0f, -0.5f);
        glVertex3f(-0.5f,  0.0f,  0.5f);
        glVertex3f( 0.5f,  0.0f,  0.5f);
        glVertex3f( 0.5f,  0.0f, -0.5f);
    glEnd();


}

// **********************************************************************
// TERRENO
//
// **********************************************************************
void DesenhaPiso()
{
    //srand(50); // usa uma semente fixa para gerar sempre as mesma cores no piso
    glPushMatrix();
    //Deixa o ch„o um pouco mais para baixo
    glTranslated(0, -0.5, 0);
    //glTranslated(CantoEsquerdo.x, CantoEsquerdo.y, CantoEsquerdo.z);
    for(int x=0; x<50;x++)
    {
        glPushMatrix();
        for(int z=0; z<25;z++)
        {
            if(matrizPiso[x][z] == true){
                DesenhaLadrilho(MediumGoldenrod, PaleGreen);
                glTranslated(0, 0, 1);
            }
        }
        glPopMatrix();
        glTranslated(1, 0, 0);
    }
    glPopMatrix();
}
void DesenhaParede()
{
    //srand(50); // usa uma semente fixa para gerar sempre as mesma cores no piso
    glPushMatrix();
    glTranslated(25, 0, 0);
    glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
    for(int x=0; x<15;x++)
    {
        glPushMatrix();
        for(int z=0; z<25;z++)
        {
            if(matrizParede[x][z] == true){
                DesenhaLadrilho(MediumGoldenrod, MediumVioletRed);
            }
            glTranslated(0, 0, 1);
        }
        glPopMatrix();
        glTranslated(1, 0, 0);
    }
    glPopMatrix();
}
void movimentarVeiculo()
{
    double velocidade = 1;
    if(andar){
        //Calculo de movimentar
        posVeiculo.soma(DirecaoCanhao.x*velocidade,DirecaoCanhao.y*velocidade,DirecaoCanhao.z*velocidade);
    }
    if(voltar){
        //Calculo de movimentar
        posVeiculo.soma(-DirecaoCanhao.x*velocidade,-DirecaoCanhao.y*velocidade,-DirecaoCanhao.z*velocidade);
    }
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

void MygluPerspective(float fieldOfView, float aspect, float zNear, float zFar )
{
    //https://stackoverflow.com/questions/2417697/gluperspective-was-removed-in-opengl-3-1-any-replacements/2417756#2417756
    // The following code is a fancy bit of math that is equivilant to calling:
    // gluPerspective( fieldOfView/2.0f, width/height , 0.1f, 255.0f )
    // We do it this way simply to avoid requiring glu.h
    //GLfloat zNear = 0.1f;
    //GLfloat zFar = 255.0f;
    //GLfloat aspect = float(width)/float(height);
    GLfloat fH = tan( float(fieldOfView / 360.0f * 3.14159f) ) * zNear;
    GLfloat fW = fH * aspect;
    glFrustum( -fW, fW, -fH, fH, zNear, zFar );
}
// **********************************************************************
//  void PosicUser()
// **********************************************************************
void PosicUser()
{

    // Define os parâmetros da projeção Perspectiva
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Define o volume de visualização sempre a partir da posicao do
    // observador
    if (ModoDeProjecao == 0)
        glOrtho(-10, 10, -10, 10, 0, 7); // Projecao paralela Orthografica
    else MygluPerspective(90,AspectRatio,0.01,50); // Projecao perspectiva

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if(pessoa3){
        gluLookAt(posOBS.x,posOBS.y,posOBS.z,
                   posVeiculo.x,0,posVeiculo.z,
                   0.0f,1.0f,0.0f);
    }
    else{
        gluLookAt(posOBS.x, posOBS.y, posOBS.z,   // Posição do Observador
                posAlvo.x,posAlvo.y,posAlvo.z,     // Posição do Alvo
                0.0f,1.0f,0.0f);
    }


}
// **********************************************************************
//  void reshape( int w, int h )
//		trata o redimensionamento da janela OpenGL
//
// **********************************************************************
void reshape( int w, int h )
{

	// Evita divisão por zero, no caso de uam janela com largura 0.
	if(h == 0) h = 1;
    // Ajusta a relação entre largura e altura para evitar distorção na imagem.
    // Veja função "PosicUser".
	AspectRatio = 1.0f * w / h;
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Seta a viewport para ocupar toda a janela
    glViewport(0, 0, w, h);
    //cout << "Largura" << w << endl;

	PosicUser();

}

// **********************************************************************
//  void display( void )
// **********************************************************************
void display( void )
{

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	DefineLuz();

	PosicUser();

	glMatrixMode(GL_MODELVIEW);

	//Cubo1 - MARCA 0,0,0
	glPushMatrix();
		glTranslatef ( 0.0f, 0.0f, 0.0f );
        glRotatef(angulo,0,1,0);
		glColor3f(0.5f,0.0f,0.0f); // Vermelho
        glutSolidCube(2);
        //DesenhaCubo(1);
	glPopMatrix();

    //Cubo 2 - MARCA O MEIO 13-25
	glPushMatrix();
		glTranslatef ( 25.0f, 0.0f, 13.0f );
		glRotatef(angulo,0,1,0);
		glColor3f(0.6156862745, 0.8980392157, 0.9803921569); // Azul claro
        glutSolidCube(2);
		//DesenhaCubo(1);
	glPopMatrix();

	//Ch„o
    DesenhaPiso();

    //Desenha a parede
    DesenhaParede();

    //Desenha o canh„o
    DesenhaCanhao();

    //testa se tem que movimentar o veiculo
    movimentarVeiculo();

    //Testa se tem colisao com a parede
    if(atirar)
    {
        Tiro();
    }
	glutSwapBuffers();
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
    case 'p':
            ModoDeProjecao = !ModoDeProjecao;
            glutPostRedisplay();
            break;
    case 'e':
            ModoDeExibicao = !ModoDeExibicao;
            init();
            glutPostRedisplay();
            break;

    //MOVIMENTACAO OBS//////////////////////////////////////////////
    case 'a':
            posOBS.z -= 2;
            break;
    case 'd':
            posOBS.z += 2;
            break;
    case 'w':
            posOBS.x += 2;
            break;
    case 's':
            posOBS.x -= 2;
            break;
    case ' ':
            posOBS.y += 1;
            break;
    case 'v':
            posOBS.y -= 1;
            break;

    //MOVIMENTACAO ALVO//////////////////////////////////////////////
    case 'j':
            posAlvo.z -= 2;
            break;
    case 'l':
            posAlvo.z += 2;
            break;

    case 'i':
            posAlvo.x += 2;
            break;
    case 'k':
            posAlvo.x -= 2;
            break;
    case 'm':
            posAlvo.y += 1;
            break;
    case 'n':
            posAlvo.y -= 1;
            break;
    //testes//////////////////////////////////////////////
     case 'c':
        if(matrizPiso[3][3] == false){
            matrizParede[3][3] = true;
            matrizPiso[3][3] = true;
            break;
        }
         matrizParede[3][3] = false;
            matrizPiso[3][3] = false;
            break;

    //CAMERA//////////////////////////////////////////////
    case '9':
            if(pessoa3){
                pessoa3 = false;
                break;
            }
            pessoa3 = true;
            break;

    //MOVIMENTACAO///////////////////////////////
    //ROTACIONAR
    case '4':
           DirecaoCanhao.rotacionaY(10);
            rotacaoVeiculo +=10;
            break;
    case '6':
            DirecaoCanhao.rotacionaY(-10);
            rotacaoVeiculo -=10;
            break;
    //ANDAR E VOLTAR
    case '8':
            if(andar){
                andar = false;
                break;
            }
            andar = true;
            break;
    case '2':
            if(voltar){
                voltar = false;
                break;
            }
            voltar = true;
            break;
    //MIRAR///////////////////////////////
    case '7':
            //DirecaoCanhao.rotacionaZ(10);
            rotacaoCanhao +=10;
            break;
    case '1':
            //DirecaoCanhao.rotacionaZ(-10);
            rotacaoCanhao -=10;
            break;
     //ATIRAR///////////////////////////////
    case '3':
            if(atirar){
                atirar = false;
                break;
            }
            atirar = true;
            break;
    case '+':
            forca++;
            break;
    case '-':
            forca--;
            break;
    //////////////////////////////////////////////
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
	glutCreateWindow    ( "Computacao Grafica - Exemplo Basico 3D" );

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




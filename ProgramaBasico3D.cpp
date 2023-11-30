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

///
#include "SOIL/SOIL.h"
#include "TextureClass.h"

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
Ponto posOBS = Ponto(10,5,-10);
//Posica do alvo
Ponto posAlvo = Ponto(25,5,13);

//Variaveis do VeÌculo
Ponto posVeiculo = Ponto(20,0,0);

//Rotacoes
double rotacaoCanhao;
double rotacaoVeiculo;

//Variaveis do projetil
Ponto posProjetil = Ponto(0,0,0);

//cria o vetor de direcao do canhao
Ponto DirecaoCanhao = Ponto(1,0,0);
double forca = 5;
boolean miraViva = false;
boolean tiroVivo = false;
double velocidadeProjetil = 1; //metros por segundo
double caminhoProjetil = 0;

//Andar e voltar
boolean andar = false;
boolean voltar = false;
double velocidade = 2.5;
//3 pessoa
boolean pessoa3 = false;

//PONTUACAO
int pontuacao = 0;

double nFrames=0;
double TempoTotal=0;
Ponto CantoEsquerdo = Ponto(-20,-1,-10);

///////////////////////////////////////TEXTURA
GLuint textureId[2];
void initTexture (void)
{
    textureId[0] = LoadTexture ("Falcao.jpg");

}



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

    initTexture();

}
// **********************************************************************
//
// **********************************************************************
void movimentarVeiculo()
{
    double velocidadeReal = velocidade/30;
    Ponto DirecaoVeiculo = Ponto(1,0,0);
    DirecaoVeiculo.rotacionaY(rotacaoVeiculo);
    if(andar){
        posVeiculo.soma(DirecaoVeiculo.x*velocidadeReal,DirecaoVeiculo.y*velocidadeReal,DirecaoVeiculo.z*velocidadeReal);
    }
    if(voltar){
        posVeiculo.soma(-DirecaoVeiculo.x*velocidadeReal,-DirecaoVeiculo.y*velocidadeReal,-DirecaoVeiculo.z*velocidadeReal);
    }

}
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
        //testa se tem que movimentar o veiculo
        movimentarVeiculo();
    }
    if (TempoTotal > 5.0)
    {
        //cout << "Tempo Acumulado: "  << TempoTotal << " segundos. " ;
       // cout << "Nros de Frames sem desenho: " << nFrames << endl;
        //cout << "FPS(sem desenho): " << nFrames/TempoTotal << endl;
        cout << "SCORE: " << pontuacao << endl;

        TempoTotal = 0;
        nFrames = 0;
    }
}
void Pontuar(int objeto){
    if(objeto == 0) // parede
    {
        pontuacao += 5;
        cout << "Destruiu uma parede: +5" << endl;
    }
    if(objeto == 1){//chao
        pontuacao -=10;
        cout << "Errou todo mundo: -10" << endl;
    }
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
    glColor3f(1,0,1);
    glScalef(3,1,2);
    glutSolidCube(1);
    glPopMatrix();
    /////

    //CANHAO
    glPushMatrix();
    //Acha o Cano em relacao ao canhao
    glTranslatef(0,0.75,0);
    //Aplica a rotacao de canhao necessaria
    glRotatef(rotacaoCanhao,0,0,1);
    //desenha o cano
    glColor3f(0,1,1);
    glScalef(1,0.5,0.5);
    glutSolidCube(1);
    glPopMatrix();
    /////

    //PROJETIL
    glPushMatrix();
    //vai na posicao atual do projetil
    glTranslatef(0,0.75,0);
    //desenha o projetil
    glColor3f(1, 0, 0);
    glutSolidSphere(0.2, 20, 20);
    glPopMatrix();

    //FIM DO VEICULO
    glColor3f(1,1,1);
    glPopMatrix();

}
void Atirar(Ponto a, Ponto b, Ponto c) //atualiza a posicao do projetil no trajeto
{
   if(caminhoProjetil<1){
    //vetor B-A
    Ponto vetorBA = b;
    vetorBA.soma((-a.x),(-a.y),(-a.z));
    posProjetil = a;
    posProjetil.soma(vetorBA.x*caminhoProjetil,vetorBA.y*caminhoProjetil,vetorBA.z*caminhoProjetil);
   }
   else{
    //vetor B-A
    Ponto vetorBC = c;
    vetorBC.soma((-b.x),(-b.y),(-b.z));

    posProjetil = b;
    posProjetil.soma(vetorBC.x*(caminhoProjetil-1.0),vetorBC.y*(caminhoProjetil-1.0),vetorBC.z*(caminhoProjetil-1.0));

   }

    caminhoProjetil += (velocidadeProjetil/10);




    if(caminhoProjetil>2.2){tiroVivo = false;caminhoProjetil=0;posProjetil = Ponto(0,0,0);Pontuar(1);}
    posProjetil.soma(posVeiculo.x,posVeiculo.y,posVeiculo.z);


}

void Mira()
{
    //OBTEM OS DADOS INICIAIS DO PROJETIL
    glPushMatrix();
    glTranslatef(posVeiculo.x,posVeiculo.y,posVeiculo.z);

    Ponto DirecaoDoCanhao = Ponto(1,0,0);
    DirecaoDoCanhao.rotacionaZ(rotacaoCanhao);
    DirecaoDoCanhao.rotacionaY(rotacaoVeiculo);

    Ponto PosicaoDoCanhao = Ponto (0, 0.75, 0);

    //calcula o ponto pos direcao de saida
    Ponto B = Ponto (PosicaoDoCanhao.x,PosicaoDoCanhao.y,PosicaoDoCanhao.z);
    B.soma(DirecaoDoCanhao.x*forca,DirecaoDoCanhao.y*forca,DirecaoDoCanhao.z*forca);

    //calcula a distancia entre o ponto inicial e o final
    double distancia = 2*forca*cos(rotacaoCanhao*3.14/180);

    //Calcula o ponto final
    Ponto C = Ponto (PosicaoDoCanhao.x,PosicaoDoCanhao.y,PosicaoDoCanhao.z);
    C.soma(distancia,0,0);
    C.rotacionaY(rotacaoVeiculo);
    //std::cout << "Distancia: " <<distancia << std::endl;

    if(tiroVivo){
        Atirar(PosicaoDoCanhao,B,C);

    }

    //DESENHA A MIRA
    //linha entre A e B
    glBegin(GL_LINES);
    glVertex3f(PosicaoDoCanhao.x, PosicaoDoCanhao.y, PosicaoDoCanhao.z);
    glVertex3f(B.x, B.y, B.z);
    glEnd();

    //Linha entre B e C
    glBegin(GL_LINES);
    glVertex3f(B.x, B.y, B.z);
    glVertex3f(C.x, C.y, C.z);
    glEnd();

    //Linha entre A e C
    glBegin(GL_LINES);
    glVertex3f(PosicaoDoCanhao.x, PosicaoDoCanhao.y, PosicaoDoCanhao.z);
    glVertex3f(C.x, C.y, C.z);
    glEnd();

glColor3f(1, 1, 1);
glPopMatrix();


}
void TestarColisao()
{
    cout << "TESTANDO COLISAO NO PONTO: "<<posProjetil.x<<", "<<posProjetil.y<<", "<<posProjetil.z<<endl;

    int um = posProjetil.y; // altura = i
    int dois = posProjetil.z; // largura = j

    if(matrizParede[um][dois] == true){ //se ainda nao foi destruido
        um = um - 1;
        dois = dois - 1;
        Pontuar(0);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {

            int linha = um + i;
            int coluna = dois + j;
            cout << "Testando: "<<linha<<", "<<coluna<<endl;
            if(linha > -1 && coluna > -1 && linha <15 && coluna <25){
                matrizParede[linha][coluna] = false;
                cout << "QUEBROU QUADRADO: "<<endl;
                Pontuar(0);
            }
        }
    }
        tiroVivo = false;
        posProjetil = Ponto(0,0,0);
    }

}
// **********************************************************************
// void DesenhaLadrilho(int corBorda, int corDentro)
// Desenha uma célula do piso.
// Eh possivel definir a cor da borda e do interior do piso
// O ladrilho tem largula 1, centro no (0,0,0) e está sobre o plano XZ
// **********************************************************************
void DesenhaLadrilho(int id)
{
        glEnable(GL_TEXTURE_2D);
        glBindTexture (GL_TEXTURE_2D, textureId[id]);

        glBegin ( GL_QUADS );
        glNormal3f(0,1,0);
        //(0.0)
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-0.5f,  0.0f, -0.5f);
        //(1.0)
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-0.5f,  0.0f,  0.5f);
        //(1.1)
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f( 0.5f,  0.0f,  0.5f);
        //(0.1)
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f( 0.5f,  0.0f, -0.5f);

        glEnd();
        glDisable(GL_TEXTURE_2D);

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
                DesenhaLadrilho(0);
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
    glRotatef(90.0f, 0.0f,0.0f, 1.0f);
    for(int x=0; x<15;x++)
    {
        glPushMatrix();
        for(int z=0; z<25;z++)
        {
            if(matrizParede[x][z] == true){
                DesenhaLadrilho(0);
            }
            else{
                glPushMatrix();
                glColor3f(0, 0, 200);
                glutSolidSphere(0.1, 20, 20);
                glColor3f(1, 1, 1);
                glPopMatrix();
            }
            glTranslated(0, 0, 1);
        }
        glPopMatrix();
        glTranslated(1, 0, 0);
    }
    glPopMatrix();
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

	//Testa colisao contra parede tem que tar na linha 25, e atÈ 15 de altura
	if(posProjetil.x > 24 && posProjetil.x < 25 && posProjetil.y<15)
    {
        cout << "TESTANDO COLISAO NO: "<<posProjetil.x<<", "<<posProjetil.y<<", "<<posProjetil.z<<endl;
        TestarColisao();

    }

	//Ch„o
    DesenhaPiso();

    //Desenha a parede
    DesenhaParede();

    //Desenha o canh„o
    DesenhaCanhao();


    //Se a mira ta ativa, imprime a mira e mostra o projetil
    if(miraViva)
    {
        Mira();
        //DESENHA O PROJETIL EM SEU CAMINHO
        glPushMatrix();
        glTranslatef(posProjetil.x,posProjetil.y,posProjetil.z);
        glColor3f(0, 0, 1);
        glutSolidSphere(0.2,20, 20);
        glColor3f(1, 1, 1);
        glPopMatrix();

        //Testa a propria destruicao
        if(caminhoProjetil>1 &&
           posProjetil.x<(posVeiculo.x+1.5)&&posProjetil.x>(posVeiculo.x-1.5)&&
           posProjetil.y<(posVeiculo.y+2)&&posProjetil.y>(posVeiculo.y-0.5)&&
           posProjetil.z<(posVeiculo.z+1)&&posProjetil.z>(posVeiculo.z-1))
        {
            cout << "Voce se destruiu"<<endl;
            exit(0);
        }

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
            rotacaoVeiculo +=10;
            if(rotacaoVeiculo==360){rotacaoVeiculo = 0;}
            break;
    case '6':
            rotacaoVeiculo += -10;
            if(rotacaoVeiculo == -10){rotacaoVeiculo +=360;}
            break;
    //ANDAR E VOLTAR
    case '8':
            if(andar){
                andar = false;
                break;
            }
            voltar = false;
            andar = true;
            break;
    case '2':
            if(voltar){
                voltar = false;
                break;
            }
            andar = false;
            voltar = true;
            break;
    case '5':
            andar = false;
            voltar = false;
            break;
    //MIRAR///////////////////////////////
    case '7':
            if(rotacaoCanhao == 90){break;}
            rotacaoCanhao +=10;
            break;
    case '1':
            if(rotacaoCanhao == 0){break;}
            rotacaoCanhao -=10;
            break;
     //ATIRAR///////////////////////////////
    case '3':
            if(miraViva){
                miraViva = false;
                break;
            }
            miraViva = true;
            break;
    case '+':

            forca++;
            break;
    case '-':
            if(forca == 0){break;}
            forca--;
            break;
    case '*':
            caminhoProjetil = 0;
            tiroVivo = true;
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




#include <fstream>
#include <iostream>
#include <cstring>  
#include <cstdlib>  
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <cmath>
#include <thread>  
#include <chrono>  

#define WINDOW_X (500)
#define WINDOW_Y (500)
#define WINDOW_NAME "Birthday Card"
#define TEXTURE_HEIGHT (512)
#define TEXTURE_WIDTH (512)

void init_GL(int argc, char *argv[]);
void init();
void set_callback_functions();

void glut_display();
void glut_keyboard(unsigned char key, int x, int y);
void glut_mouse(int button, int state, int x, int y);
void glut_motion(int x, int y);
void glut_idle();
void set_texture();
void read_file(char** arr, int max_lines);  

void display_string(char *str, void *font);
void draw_pyramid();
void draw_triangle();
void draw_square_edge();
void draw_cube();
void draw_square();
void draw_cylinder(float radius, float height, int sides);
void draw_balloon();
void draw_mailedge_yoko();
void draw_mailedge_tate();
void draw_ribbon();

void page1_option1();
void page2_option0();
void page2_option01();
void page2_option1();
void page2_option2();
void page2_option3();
void page2_option4();
void page2_option5();
void page3_option1();
void page4_option1();
void page5_option1();
void page6_option0();

void updateBalloon(int value);
void updateBalloonZ(int value);

void display_fixed_elements();
void display_fixed_text_free(const char* text, int x, int y, void* font);

double g_angle1 = 0.0;
double g_angle2 = 0.0;
double g_distance = 10.0;
bool g_isLeftButtonOn = false;
bool g_isRightButtonOn = false;
char** arr;
int pagenum;
int page2_num;
int page5_num;
int music_flag;
int last_pagenum; 

GLuint g_TextureHandles[6] = {0,0,0,0,0,0};
GLuint g_TextureHandles2[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
cv::VideoCapture cap;
std::string input_video;
std::string audio_page;
sf::Music music;

const float PHI = (1.0 + sqrt(5.0)) / 2.0; 
const float SIZE = 1.0;

int main(int argc, char *argv[]) {
	const int MAX_LINES = 15; 
    arr = new char*[MAX_LINES];  
	pagenum = 0;
	page2_num = 0;
	page5_num = 0;
	last_pagenum = -1;
	read_file(arr,MAX_LINES);

	init_GL(argc,argv);

	init();

	set_callback_functions();

	glutMainLoop(); 

    delete[] arr;  
    return 0;
}

void read_file(char** arr, int max_lines) {
    std::ifstream ifs; 
    ifs.open("template.txt");  
    if (ifs.fail()) {  
        std::cerr << "ファイルを開けません\n";
        exit(1);
    }
    char buf[256];  
    int linenum = 0; 

    while (ifs.getline(buf, sizeof(buf)) && linenum < max_lines) {  
        arr[linenum] = strdup(buf);  
        linenum++;  
    }

    for (int i = linenum; i < max_lines; i++) {
        arr[i] = nullptr;  
    }
}	


void init_GL(int argc, char *argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_X,WINDOW_Y);
	glutCreateWindow(WINDOW_NAME);
	glutIdleFunc(glut_idle);
}

void init(){
	if (arr[0] != nullptr && arr[0][0] == '1') {
		glClearColor(1.0, 1.0, 0.8, 1.0f);
    }
	else if(arr[0] != nullptr && arr[0][0] == '2') {
		glClearColor(0.8, 0.8, 1.0, 1.0f);
    }
	else if (arr[0] != nullptr && arr[0][0] == '3') {
		glClearColor(0.8, 1.0, 1.0, 1.0f);
    }
	else if (arr[0] != nullptr && arr[0][0] == '4') {
		glClearColor(1.0, 0.8, 1.0, 1.0f);
    }
	else if (arr[0] != nullptr && arr[0][0] == '5') {
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    }

	glGenTextures(6, g_TextureHandles);
  	for(int i = 0; i < 6; i++){  
    	glBindTexture(GL_TEXTURE_2D, g_TextureHandles[i]);
    	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  	}
  	set_texture();
	glGenTextures(12, g_TextureHandles2);
  	for(int i = 0; i < 12; i++){  
    	glBindTexture(GL_TEXTURE_2D, g_TextureHandles2[i]);
    	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  	}
  	set_texture();
  	input_video = arr[11];
	audio_page = arr[4]; 
	cap.open(input_video);
  	if(!cap.isOpened()){
    	printf("no video\n");
  	}
	music_flag = 0;
}
	
void set_callback_functions(){
	glutDisplayFunc(glut_display);
	glutKeyboardUpFunc(glut_keyboard);
	glutMouseFunc(glut_mouse);
	glutMotionFunc(glut_motion);
	glutPassiveMotionFunc(glut_motion);
}

void glut_keyboard(unsigned char key, int x, int y){
	switch(key){
		case 'q':
		case 'Q':
		case '\033':
			exit(0);
		case 'h':
			pagenum = (pagenum + 1) % 6; 
			break;
		case 'g' :
			pagenum = (pagenum - 1) % 6; 
			break;
		case '1' :
			if(pagenum == 1){
				page2_num = 1;
				break;
			}
		case '2' :
			if(pagenum == 1){
				page2_num = 2;
				break;
			}
		case '3' :
			if(pagenum == 1){
				page2_num = 3;
				break;
			}
		case '4' :
			if(pagenum == 1){
				page2_num = 4;
				break;
			}
		case '5' :
			if(pagenum == 1){
				page2_num = 5;
				break;
			}
		case 'b' :
			if(pagenum == 4){
				page5_num = (page5_num - 1) % 24;
				break;
			}
		case 'n' :
			if(pagenum == 4){
				page5_num = (page5_num + 1) % 24;
				break;
			}
	}
	glutPostRedisplay();
}

void glut_mouse(int button, int state, int x, int y){
	if(button == GLUT_LEFT_BUTTON){
		if(state == GLUT_UP){
			g_isLeftButtonOn = false;
		}
		else if(state == GLUT_DOWN){
			g_isLeftButtonOn = true;
		}
	}
	if(button == GLUT_RIGHT_BUTTON){
		if(state == GLUT_UP){
			g_isRightButtonOn = false;
		}
		else if(state == GLUT_DOWN){
			g_isRightButtonOn = true;
		}
	}
}

void glut_motion(int x, int y){
	static int px = -1, py = -1;
	if(g_isLeftButtonOn == true){
		if(px >= 0 && py >= 0){
			g_angle1 += (double)-(x - px)/20;
			g_angle2 += (double)(y - py)/20;
		}
		px = x;
		py = y;
	}
	else if(g_isRightButtonOn == true){
		if(px >= 0 && py >= 0){
			g_distance += (double)(y - py)/20;
		}
		px = x;
		py = y;
	}
	else{
		px = -1;
		py = -1;
	}
	glutPostRedisplay();
}

void glut_idle(){
	if((pagenum != 1)&&(pagenum != 4)){
		glutPostRedisplay();
	}
}

void glut_display(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, 1.0, 0.1, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (pagenum != last_pagenum) {
        g_angle1 = 0.0; 
        g_angle2 = 0.0;
        g_distance = 10.0;
        last_pagenum = pagenum;
    }

	if (cos(g_angle2)>0){
		gluLookAt(g_distance * cos(g_angle2) * sin(g_angle1),
			g_distance * sin(g_angle2),
			g_distance * cos(g_angle2) * cos(g_angle1),
			0.0, 0.0, 0.0, 0.0, 1.0,0.0);
	}
	else {
		gluLookAt(g_distance * cos(g_angle2) * sin(g_angle1),
			g_distance * sin(g_angle2),
			g_distance * cos(g_angle2) * cos(g_angle1),
			0.0, 0.0, 0.0, 0.0, -1.0,0.0);
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	if (pagenum == 0){
        page1_option1();
	}
	if (pagenum == 1){
		static int counter = 0;
		if(page2_num == 0) {
			if(arr[10][0]=='1'){
				page2_option0();
			}
        	else if(arr[10][0]=='2'){
				page2_option01();
			}
    	}
		else if (page2_num == 1){
			page2_option1();
  			counter++;
  			if(counter > 100){
				page2_num = 0;
				counter = 0;
  			} 
			glutPostRedisplay();
		}
		else if (page2_num == 2){
			page2_option2();
			counter++;
  			if(counter > 100){
				page2_num = 0;
				counter = 0;
  			} 
			glutPostRedisplay();
		}
		else if (page2_num == 3){
			page2_option3();
			counter++;
  			if(counter > 100){
				page2_num = 0;
				counter = 0;
  			} 
			glutPostRedisplay();
		}
		else if (page2_num == 4){
			page2_option4();
			counter++;
  			if(counter > 100){
				page2_num = 0;
				counter = 0;
  			} 
			glutPostRedisplay();
		}
		else if (page2_num == 5){
			page2_option5();
			counter++;
  			if(counter > 100){
				page2_num = 0;
				counter = 0;
  			} 
			glutPostRedisplay();
		}	
	}
	if (pagenum == 2){
        page3_option1();
	}
	if (pagenum == 3){
		page4_option1();
	}
	if (pagenum == 4){
		page5_option1();
	}
	if (pagenum == 5){
		page6_option0();
	}
	glDisable(GL_DEPTH_TEST);
	glutSwapBuffers();
}

void display_string(char *str, void *font){
    unsigned int i;
    for(i = 0; i < strlen(str); i++) 
        glutBitmapCharacter(font, str[i]); 
}

void display_fixed_elements() {
    glPushMatrix();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, WINDOW_X, 0, WINDOW_Y, -1, 1); 

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(0.7, 0.7, 0.7);
    glRasterPos2f(WINDOW_X - 52, WINDOW_Y - 480); 
    display_string("h", GLUT_BITMAP_HELVETICA_18);

    glPushMatrix();
    glColor3d(0.7, 0.7, 0.7);
    glTranslated(WINDOW_X - 30, WINDOW_Y- 485, 0); 
    glScalef(20, 20, 1);
    draw_triangle();
    glPopMatrix();

    glPushMatrix();
    glColor3d(0.7, 0.7, 0.7);
    glTranslated(WINDOW_X  - 60, WINDOW_Y- 488, 0); 
    glScalef(25, 25, 1); 
    draw_square_edge();
    glPopMatrix();

    glColor3f(0.7, 0.7, 0.7);
    glRasterPos2f(44, WINDOW_Y - 480); 
    display_string("g", GLUT_BITMAP_HELVETICA_18);

    glPushMatrix();
    glColor3d(0.7, 0.7, 0.7);
    glTranslated(30, WINDOW_Y - 485, 0); 
    glScalef(20, 20, 1); 
    glRotatef(-180, 0.0, 1.0, 0.0); 
    draw_triangle();
    glPopMatrix();

    glPushMatrix();
    glColor3d(0.7, 0.7, 0.7);
    glTranslated(40, WINDOW_Y - 488, 0);
    glScalef(25, 25, 1); 
    draw_square_edge();
    glPopMatrix();

    glPopMatrix(); 
    glMatrixMode(GL_PROJECTION);
    glPopMatrix(); 
    glMatrixMode(GL_MODELVIEW);

    glPopMatrix();
}

void display_fixed_text_free(const char* text, int x, int y, void* font) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, WINDOW_X, 0, WINDOW_Y, -1, 1); 

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glRasterPos2i(x, y);  
    for (const char* c = text; *c != '\0'; ++c) {
        glutBitmapCharacter(font, *c);  
    }

    glPopMatrix(); 
    glMatrixMode(GL_PROJECTION);
    glPopMatrix(); 
    glMatrixMode(GL_MODELVIEW);
}

void draw_pyramid(){
	GLdouble pointO[] = {0.0, 1.0, 0.0};
	GLdouble pointA[] = {1.5, -1.0, 1.5};
	GLdouble pointB[] = {-1.5, -1.0, 1.5};
	GLdouble pointC[] = {-1.5, -1.0, -1.5};
	GLdouble pointD[] = {1.5, -1.0, -1.5};

	glBegin(GL_TRIANGLES);
	glVertex3dv(pointO);
	glVertex3dv(pointA);
	glVertex3dv(pointB);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex3dv(pointO);
	glVertex3dv(pointB);
	glVertex3dv(pointC);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex3dv(pointO);
	glVertex3dv(pointC);
	glVertex3dv(pointD);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex3dv(pointO);
	glVertex3dv(pointD);
	glVertex3dv(pointA);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3dv(pointA);
	glVertex3dv(pointB);
	glVertex3dv(pointC);
	glVertex3dv(pointD);
	glEnd();
}

void draw_cylinder(float radius, float height, int sides) {
    double pi = 3.14159265358979323846;

    glBegin(GL_POLYGON);
    for (int i = 0; i < sides; ++i) {
        double angle = 2 * pi * i / sides;
        glVertex3f(radius * cos(angle), height, radius * sin(angle));
    }
    glEnd();

    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= sides; ++i) {
        double angle = 2 * pi * i / sides;
        glVertex3f(radius * cos(angle), height, radius * sin(angle));
        glVertex3f(radius * cos(angle), -height, radius * sin(angle));
    }
    glEnd();

    glBegin(GL_POLYGON);
    for (int i = sides; i >= 0; --i) {
        double angle = 2 * pi * i / sides;
        glVertex3f(radius * cos(angle), -height, radius * sin(angle));
    }
    glEnd();
}

void draw_ballon(){
	glPushMatrix();
  	glTranslated(0.0, 1.5, 0.0);
  	glutSolidSphere(0.5, 20, 20);
  	glPopMatrix();

	glPushMatrix();
	glTranslated(0.0, 0.9, 0.0);
	glRotatef(-90,1.0,0.0,0.0);
	glutSolidCone(0.1,0.2,20,20);
  	glPopMatrix();

	glPushMatrix();
    glTranslated(0.0, 0.4, 0.0);
	glColor3d(0.6, 0.6, 0.6);
    draw_cylinder(0.01f, 1.0f, 36); 
	glPopMatrix();
}

void draw_triangle(){
	GLdouble pointA[] = {0.0, 1.0, 0.0};
	GLdouble pointB[] = {0.0, 0.0, 0.0};
	GLdouble pointC[] = {1.0, 0.5, 0.0};	
	glBegin(GL_TRIANGLES);
	glVertex3dv(pointA);
	glVertex3dv(pointB);
	glVertex3dv(pointC);
	glEnd();
}

void draw_square_edge(){
    glBegin(GL_LINES);

    glVertex3d(1.0, 0.0, 0.0);
    glVertex3d(0.0, 0.0, 0.0);

    glVertex3d(1.0, 0.0, 0.0);
    glVertex3d(1.0, 1.0, 0.0);

    glVertex3d(1.0, 1.0, 0.0);
    glVertex3d(0.0, 1.0, 0.0);

    glVertex3d(0.0, 1.0, 0.0);
    glVertex3d(0.0, 0.0, 0.0);
    glEnd(); 
}

void draw_cube(){
	GLdouble pointO[] = {-1.0, 1.0, -1.0};
	GLdouble pointA[] = {1.0, 1.0, -1.0};
	GLdouble pointB[] = {-1.0, -1.0, -1.0};
	GLdouble pointC[] = {1.0, -1.0, -1.0};
	GLdouble pointD[] = {-1.0, 1.0, 1.0};
    GLdouble pointE[] = {1.0, 1.0, 1.0};
	GLdouble pointF[] = {-1.0, -1.0, 1.0};
	GLdouble pointG[] = {1.0, -1.0, 1.0};

	glBegin(GL_POLYGON);
	glVertex3dv(pointO);
	glVertex3dv(pointB);
	glVertex3dv(pointC);
	glVertex3dv(pointA);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3dv(pointA);
	glVertex3dv(pointC);
	glVertex3dv(pointG);
	glVertex3dv(pointE);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3dv(pointE);
	glVertex3dv(pointG);
	glVertex3dv(pointF);
	glVertex3dv(pointD);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3dv(pointD);
	glVertex3dv(pointF);
	glVertex3dv(pointB);
	glVertex3dv(pointO);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3dv(pointO);
	glVertex3dv(pointA);
	glVertex3dv(pointE);
	glVertex3dv(pointD);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3dv(pointB);
	glVertex3dv(pointC);
	glVertex3dv(pointG);
	glVertex3dv(pointF);
	glEnd();
}

void draw_square(){
    GLdouble pointA[] = {-0.5, 0.5, 0.0};
    GLdouble pointB[] = {0.5, 0.5, 0.0};
    GLdouble pointC[] = {0.5, -0.5, 0.0};
    GLdouble pointD[] = {-0.5, -0.5, 0.0};
    glBegin(GL_POLYGON);
    glVertex3dv(pointA);
    glVertex3dv(pointB);
    glVertex3dv(pointC);
    glVertex3dv(pointD);
    glEnd();
}

void draw_ribbon(){
	glColor3d(0.741, 0.718, 0.420); 

	glPushMatrix();
  	glTranslated(-1.5, 1.0, 0.51);
	glScalef(0.3,1.02,0.1);
  	draw_square();
  	glPopMatrix();

	glPushMatrix();
  	glTranslated(-1.5, 1.0, -0.51);
	glScalef(0.3,1.02,0.1);
  	draw_square();
  	glPopMatrix();

	glPushMatrix();
  	glTranslated(-0.99, 1.0, 0.0);
	glRotatef(90,0.0,1.0,0.0);
	glScalef(0.3,1.02,0.1);
  	draw_square();
  	glPopMatrix();

	glPushMatrix();
  	glTranslated(-2.01, 1.0, 0.0);
	glRotatef(-90,0.0,1.0,0.0);
	glScalef(0.3,1.02,0.1);
  	draw_square();
  	glPopMatrix();

	glPushMatrix();
  	glTranslated(-1.5, 1.51, 0.0);
	glRotatef(-90,1.0,0.0,0.0);
	glScalef(0.3,1.02,0.1);
  	draw_square();
  	glPopMatrix();

	glPushMatrix();
  	glTranslated(-1.5, 0.49, 0.0);
	glRotatef(-90,1.0,0.0,0.0);
	glScalef(0.3,1.02,0.1);
  	draw_square();
  	glPopMatrix();

	glPushMatrix();
  	glTranslated(-1.5, 1.51, 0.0);
	glRotatef(-90,1.0,0.0,0.0);
	glScalef(1.02,0.3,0.1);
  	draw_square();
  	glPopMatrix();

	glPushMatrix();
  	glTranslated(-1.5, 0.49, 0.0);
	glRotatef(-90,1.0,0.0,0.0);
	glScalef(1.02,0.3,0.1);
  	draw_square();
  	glPopMatrix();

	glPushMatrix();
  	glTranslated(-1.65, 1.65, -0.1);
	glRotatef(-45,0.0,1.0,0.0);
	glRotatef(-135,0.0,0.0,1.0);
	glScalef(0.1,0.3,0.1);
  	draw_pyramid();
  	glPopMatrix();

	glPushMatrix();
  	glTranslated(-1.65, 1.65, 0.1);
	glRotatef(45,0.0,1.0,0.0);
	glRotatef(-135,0.0,0.0,1.0);
	glScalef(0.1,0.3,0.1);
  	draw_pyramid();
  	glPopMatrix();

	glPushMatrix();
  	glTranslated(-1.4, 1.65, 0.1);
	glRotatef(-45,0.0,1.0,0.0);
	glRotatef(135,0.0,0.0,1.0);
	glScalef(0.1,0.3,0.1);
  	draw_pyramid();
  	glPopMatrix();

	glPushMatrix();
  	glTranslated(-1.4, 1.7, -0.1);
	glRotatef(45,0.0,1.0,0.0);
	glRotatef(135,0.0,0.0,1.0);
	glScalef(0.1,0.3,0.1);
  	draw_pyramid();
  	glPopMatrix();
}

void draw_mailedge_yoko(){
	GLdouble rightup[] = {0.2, 0.1, 0.0};
	GLdouble rightdown[] = {0.15, 0.0, 0.0};
	GLdouble leftdown[] = {-0.05, 0.0, 0.0};
	GLdouble leftup[] = {0.0, 0.1, 0.0};
	glBegin(GL_POLYGON);
	glVertex3dv(rightup);
	glVertex3dv(rightdown);
	glVertex3dv(leftdown);
	glVertex3dv(leftup);
	glEnd();
}

void draw_mailedge_tate(){
	GLdouble rightup[] = {0.15, 0.2, 0.0};
	GLdouble rightdown[] = {0.1, 0.0, 0.0};
	GLdouble leftdown[] = {0.0, 0.0, 0.0};
	GLdouble leftup[] = {0.05, 0.2, 0.0};
	glBegin(GL_POLYGON);
	glVertex3dv(rightup);
	glVertex3dv(rightdown);
	glVertex3dv(leftdown);
	glVertex3dv(leftup);
	glEnd();
}

void drawDodecahedron() {
	float vertices[20][3] = {
    	{ SIZE, SIZE, SIZE },
    	{ SIZE, SIZE, -SIZE },
    	{ SIZE, -SIZE, SIZE },
    	{ SIZE, -SIZE, -SIZE },
    	{ -SIZE, SIZE, SIZE },
    	{ -SIZE, SIZE, -SIZE },
    	{ -SIZE, -SIZE, SIZE },
    	{ -SIZE, -SIZE, -SIZE },
    	{ 0, PHI * SIZE, SIZE / PHI },
   		{ 0, PHI * SIZE, -SIZE / PHI },
   		 { 0, -PHI * SIZE, SIZE / PHI },
    	{ 0, -PHI * SIZE, -SIZE / PHI },
    	{ SIZE / PHI, 0, PHI * SIZE },
    	{ SIZE / PHI, 0, -PHI * SIZE },
    	{ -SIZE / PHI, 0, PHI * SIZE },
    	{ -SIZE / PHI, 0, -PHI * SIZE },
    	{ PHI * SIZE, SIZE / PHI, 0 },
    	{ PHI * SIZE, -SIZE / PHI, 0 },
    	{ -PHI * SIZE, SIZE / PHI, 0 },
    	{ -PHI * SIZE, -SIZE / PHI, 0 }
	};

	int faces[12][5] = {
    	{ 0, 8, 9, 1, 16 },
    	{ 0, 16, 17, 2, 12 },
    	{ 12, 2, 10, 6, 14}, 
    	{ 3, 17, 16, 1, 13},
    	{14, 4, 8, 0, 12},
    	{ 7, 11, 3, 13, 15 }, 
    	{ 1, 9, 5, 15, 13 },
    	{ 2, 17, 3, 11, 10 },
    	{ 18, 5, 9, 8, 4 },
    	{ 6, 10, 11, 7, 19 },
    	{ 15,5,18, 19,7},
    	{4, 14, 6, 19, 18}
	};

	float colors[12][3] = {
    	{ 1.0f, 0.0f, 0.0f },  
    	{ 0.0f, 1.0f, 0.0f },  
    	{ 0.0f, 0.0f, 1.0f },  
    	{ 1.0f, 1.0f, 0.0f },   
    	{ 0.0f, 1.0f, 1.0f },  
    	{ 1.0f, 0.0f, 1.0f },  
    	{ 0.5f, 0.0f, 0.5f },  
    	{ 1.0f, 0.5f, 0.0f }, 
    	{ 0.5f, 1.0f, 0.0f },  
    	{ 0.5f, 0.5f, 1.0f },  
    	{ 0.5f, 1.0f, 1.0f },  
    	{ 1.0f, 0.5f, 0.5f }   
	};

    for (int i = 0; i < 12; i++) {
        glColor3fv(colors[i]);
        glBegin(GL_POLYGON);
        for (int j = 0; j < 5; j++) {
            int vertexIndex = faces[i][j];
            glVertex3fv(vertices[vertexIndex]);
        }
        glEnd(); 
	}

}

void page1_option1(){
	music.stop();
	music_flag = 0;
	GLdouble paper_rightup[] = {2.5, 1.5, 0.0};
	GLdouble paper_rightdown[] = {2.5, -1.5, 0.0};
	GLdouble paper_leftdown[] = {-2.5, -1.5, 0.0};
	GLdouble paper_leftup[] = {-2.5, 1.5, 0.0};

	GLdouble stamp_rightup[] = {1.3, 1.1, 0.1};
	GLdouble stamp_rightdown[] = {1.3, 0.6, 0.1};
	GLdouble stamp_leftdown[] = {2.0, 0.6, 0.1};
	GLdouble stamp_leftup[] = {2.0, 1.1, 0.1};

	glPushMatrix();
	if (arr[1] != nullptr && arr[1][0] == '1') {
        glColor3d(1.0, 1.0 ,0.8);
    }
	else if(arr[1] != nullptr && arr[1][0] == '2') {
        glColor3d(0.8, 0.8, 1.0);
    }
	else if(arr[1] != nullptr && arr[1][0] == '3') {
        glColor3d(0.8, 1.0, 1.0);
    }
	else if(arr[1] != nullptr && arr[1][0] == '4') {
        glColor3d(1.0, 0.8, 1.0);
    }
		
	glBegin(GL_POLYGON);
	glVertex3dv(paper_rightup);
	glVertex3dv(paper_rightdown);
	glVertex3dv(paper_leftdown);
	glVertex3dv(paper_leftup);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	if (arr[1] != nullptr && arr[1][0] == '1') {
        glColor3d(1.0 , 1.0 , 0.5);
    }
	else if(arr[1] != nullptr && arr[1][0] == '2') {
        glColor3d(0.5, 0.5, 1.0);
    }
	else if(arr[1] != nullptr && arr[1][0] == '3') {
        glColor3d(0.0, 1.0 , 1.0);
    }
	else if(arr[1] != nullptr && arr[1][0] == '4') {
        glColor3d(1.0 , 0.5 , 1.0);
    }

	glBegin(GL_POLYGON);
	glVertex3dv(stamp_rightup);
	glVertex3dv(stamp_rightdown);
	glVertex3dv(stamp_leftdown);
	glVertex3dv(stamp_leftup);
	glEnd();
	glPopMatrix();

	for(int i = 0; i < 8; i++){
		glPushMatrix();
		glColor3d(0.0,0.0,1.0); 
   		glTranslated(2.15-0.6*i, 1.3, 0.1);
		draw_mailedge_yoko();
  		glPopMatrix();

		glPushMatrix();
		glColor3d(1.0,0.0,0.0); 
   		glTranslated(1.85-0.6*i, 1.3, 0.1);
		draw_mailedge_yoko();
  		glPopMatrix();
	}
	for(int i = 0; i < 8; i++){
		glPushMatrix();
		glColor3d(1.0,0.0,0.0); 
   		glTranslated(2.15-0.6*i, -1.4, 0.1);
		draw_mailedge_yoko();
  		glPopMatrix();

		glPushMatrix();
		glColor3d(0.0,0.0,1.0); 
   		glTranslated(1.85-0.6*i, -1.4, 0.1);
		draw_mailedge_yoko();
  		glPopMatrix();
	}
	for(int i = 0; i < 4; i++){
		glPushMatrix();
		glColor3d(0.0,0.0,1.0); 
   		glTranslated(-2.4, 0.95-0.6*i, 0.1);
		draw_mailedge_tate();
  		glPopMatrix();

		glPushMatrix();
		glColor3d(1.0,0.0,0.0); 
   		glTranslated(-2.4, 0.65-0.6*i, 0.1);
		draw_mailedge_tate();
  		glPopMatrix();
	}
	for(int i = 0; i < 4; i++){
		glPushMatrix();
		glColor3d(1.0,0.0,0.0); 
   		glTranslated(2.25, 0.95-0.6*i, 0.1);
		draw_mailedge_tate();
  		glPopMatrix();

		glPushMatrix();
		glColor3d(0.0,0.0,1.0); 
   		glTranslated(2.25, 0.65-0.6*i, 0.1);
		draw_mailedge_tate();
  		glPopMatrix();
	}

    if (arr[1] != nullptr) {
		glColor3f(0.0, 0.0, 0.0);
		glRasterPos3d(0.6, -0.5, 0.1);
		display_string("To", GLUT_BITMAP_TIMES_ROMAN_24);
		glRasterPos3d(0.8, -0.8, 0.1);
		display_string(arr[2], GLUT_BITMAP_TIMES_ROMAN_24);
		glPopMatrix();
    }
	if (arr[2] != nullptr) {
		glColor3f(0.0, 0.0, 0.0);
		glRasterPos3d(-1.8, 0.8, 0.1);
		display_string("From", GLUT_BITMAP_TIMES_ROMAN_24);
		glRasterPos3d(-1.6, 0.5, 0.1);
		display_string(arr[3], GLUT_BITMAP_TIMES_ROMAN_24);
    }

	glColor3f(1.0, 0.0, 0.0);
	glRasterPos3d(-1.8, -1.0, 0.1);
	display_string("VIA", GLUT_BITMAP_TIMES_ROMAN_24);		
	glRasterPos3d(-1.3, -1.0, 0.1);
	display_string("AIR", GLUT_BITMAP_TIMES_ROMAN_24);
	glRasterPos3d(-0.8, -1.0, 0.1);
	display_string("MAIL", GLUT_BITMAP_TIMES_ROMAN_24);

	glPushMatrix();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, WINDOW_X, 0, WINDOW_Y, -1, 1); 

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(0.7, 0.7, 0.7);
    glRasterPos2f(WINDOW_X - 52, WINDOW_Y - 480); 
    display_string("h", GLUT_BITMAP_HELVETICA_18);

    glPushMatrix();
    glColor3d(0.7, 0.7, 0.7);
    glTranslated(WINDOW_X - 30, WINDOW_Y- 485, 0); 
    glScalef(20, 20, 1); 
    draw_triangle();
    glPopMatrix();

    glPushMatrix();
    glColor3d(0.7, 0.7, 0.7);
    glTranslated(WINDOW_X  - 60, WINDOW_Y- 488, 0); 
    glScalef(25, 25, 1); 
    draw_square_edge();
    glPopMatrix();

    glPopMatrix(); 
    glMatrixMode(GL_PROJECTION);
    glPopMatrix(); 
    glMatrixMode(GL_MODELVIEW);
}

void page2_option0(){
	if(music_flag == 0){
		music_flag = 1;
		if (!music.openFromFile(audio_page)) {
        	std::cerr << "音声ファイルの読み込みに失敗しました" << std::endl;
    	}
		music.setLoop(true);
    	music.play();
	}

	glPushMatrix();
	glColor3d(1.0, 0.0, 1.0); 
	glTranslated(-2.0, 1.5, 0.0); 
	glutSolidSphere(0.5, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glColor3d(1.0, 0.0, 1.0);
	glTranslated(-2.0, 0.9, 0.0); 
	glRotatef(-90, 1.0, 0.0, 0.0);
	glutSolidCone(0.1, 0.2, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-2.0, 0.4, 0.0); 
	glColor3d(0.6, 0.6, 0.6);
	draw_cylinder(0.01f, 0.5f, 36);
	glPopMatrix();

	glPushMatrix();
  	glColor3d(0.5, 0.0, 1.0); 
  	glTranslated(0.0, 1.5, 0.0);
  	glutSolidSphere(0.5, 20, 20);
  	glPopMatrix();

	glPushMatrix();
  	glColor3d(0.5, 0.0, 1.0);;
	glTranslated(0.0, 0.9, 0.0);
	glRotatef(-90,1.0,0.0,0.0);
	glutSolidCone(0.1,0.2,20,20);
  	glPopMatrix();

	glPushMatrix();
    glTranslated(0.0, 0.4, 0.0);
    glColor3d(0.6, 0.6, 0.6);
    draw_cylinder(0.01f, 0.5f, 36); 
	glPopMatrix();

	glPushMatrix();
  	glColor3d(0.0, 1.0, 0.5);
  	glTranslated(2.0, 1.5, 0.0);
  	glutSolidSphere(0.5, 20, 20);
  	glPopMatrix();

	glPushMatrix();
  	glColor3d(0.0, 1.0, 0.5); 
	glTranslated(2.0, 0.9, 0.0);
	glRotatef(-90,1.0,0.0,0.0);
	glutSolidCone(0.1,0.2,20,20);
  	glPopMatrix();

	glPushMatrix();
    glTranslated(2.0, 0.4, 0.0);
    glColor3d(0.6, 0.6, 0.6);
    draw_cylinder(0.01f, 0.5f, 36); 
	glPopMatrix();
	
	glPushMatrix();
  	glColor3d(1.0, 1.0, 0.0); 
  	glTranslated(-1.0, -0.5, 0.0);
  	glutSolidSphere(0.5, 20, 20);
  	glPopMatrix();

	glPushMatrix();
  	glColor3d(1.0, 1.0, 0.0); 
	glTranslated(-1.0, -1.1, 0.0);
	glRotatef(-90,1.0,0.0,0.0);
	glutSolidCone(0.1,0.2,20,20);
  	glPopMatrix();

	glPushMatrix();
    glTranslated(-1.0, -1.6, 0.0);
    glColor3d(0.6, 0.6, 0.6);
    draw_cylinder(0.01f, 0.5f, 36); 
	glPopMatrix();

	glPushMatrix();
  	glColor3d(0.0, 0.0, 1.0); 
  	glTranslated(1.0, -0.5, 0.0);
  	glutSolidSphere(0.5, 20, 20);
  	glPopMatrix();

	glPushMatrix();
  	glColor3d(0.0, 0.0, 1.0); 
	glTranslated(1.0, -1.1, 0.0);
	glRotatef(-90,1.0,0.0,0.0);
	glutSolidCone(0.1,0.2,20,20);
  	glPopMatrix();

	glPushMatrix();
    glTranslated(1.0, -1.6, 0.0);
    glColor3d(0.6, 0.6, 0.6);
    draw_cylinder(0.01f, 0.5f, 36); 
	glPopMatrix();

	glColor3f(0.0, 0.0, 0.0);
	glRasterPos3d(-2.3, 1.4, 0.5);
	display_string("Click '1'", GLUT_BITMAP_TIMES_ROMAN_24);

	glColor3f(0.0, 0.0, 0.0);
	glRasterPos3d(-0.4, 1.4, 0.5);
	display_string("Click '2'", GLUT_BITMAP_TIMES_ROMAN_24);

	glColor3f(0.0, 0.0, 0.0);
	glRasterPos3d(1.5, 1.4, 0.5);
	display_string("Click '3'", GLUT_BITMAP_TIMES_ROMAN_24);

	glColor3f(0.0, 0.0, 0.0);
	glRasterPos3d(-1.35, -0.5, 0.5);
	display_string("Click '4'", GLUT_BITMAP_TIMES_ROMAN_24);

	glColor3f(0.0, 0.0, 0.0);
	glRasterPos3d(0.55, -0.5, 0.5);
	display_string("Click '5'", GLUT_BITMAP_TIMES_ROMAN_24);

	display_fixed_elements();
}

void page2_option01(){
	if(music_flag == 0){
		music_flag = 1;
		if (!music.openFromFile(audio_page)) {
        	std::cerr << "音声ファイルの読み込みに失敗しました" << std::endl;
    	}
		music.setLoop(true);
    	music.play();
	}

	glPushMatrix();
  	glColor3d(1.0, 0.0, 1.0); 
  	glTranslated(-1.5, 1.0, 0.0);
	glScalef(0.5,0.5,0.5);
  	draw_cube();
  	glPopMatrix();
	glPushMatrix();
  	draw_ribbon();
  	glPopMatrix();

	glPushMatrix();
  	glColor3d(0.5, 0.0, 1.0); 
  	glTranslated(0.0, 1.0, 0.0);
	glScalef(0.5,0.5,0.5);
  	draw_cube();
  	glPopMatrix();
	glPushMatrix();
	glTranslated(1.5, 0.0, 0.0);
  	draw_ribbon();
  	glPopMatrix();

	glPushMatrix();
  	glColor3d(0.0, 1.0, 0.5); 
  	glTranslated(1.5, 1.0, 0.0);
	glScalef(0.5,0.5,0.5);
  	draw_cube();
  	glPopMatrix();
	glPushMatrix();
	glTranslated(3.0, 0.0, 0.0);
  	draw_ribbon();
  	glPopMatrix();

	glPushMatrix();
  	glColor3d(1.0, 1.0, 0.0); 
  	glTranslated(-0.75, -1.0, 0.0);
	glScalef(0.5,0.5,0.5);
  	draw_cube();
  	glPopMatrix();
	glPushMatrix();
	glTranslated(0.75, -2.0, 0.0);
  	draw_ribbon();
  	glPopMatrix();

	glPushMatrix();
  	glColor3d(0.0, 0.0, 1.0); 
  	glTranslated(0.75, -1.0, 0.0);
	glScalef(0.5,0.5,0.5);
  	draw_cube();
  	glPopMatrix();
	glPushMatrix();
	glTranslated(2.25, -2.0, 0.0);
  	draw_ribbon();
  	glPopMatrix();

	glColor3f(0.0, 0.0, 0.0);
	glRasterPos3d(-1.9, 1.0, 0.6);
	display_string("Click '1'", GLUT_BITMAP_TIMES_ROMAN_24);

	glColor3f(0.0, 0.0, 0.0);
	glRasterPos3d(-0.4, 1.0, 0.6);
	display_string("Click '2'", GLUT_BITMAP_TIMES_ROMAN_24);

	glColor3f(0.0, 0.0, 0.0);
	glRasterPos3d(1.1, 1.0, 0.6);
	display_string("Click '3'", GLUT_BITMAP_TIMES_ROMAN_24);

	glColor3f(0.0, 0.0, 0.0);
	glRasterPos3d(-1.15, -1.0, 0.6);
	display_string("Click '4'", GLUT_BITMAP_TIMES_ROMAN_24);

	glColor3f(0.0, 0.0, 0.0);
	glRasterPos3d(0.35, -1.0, 0.6);
	display_string("Click '5'", GLUT_BITMAP_TIMES_ROMAN_24);

	glColor3f(0.7, 0.7, 0.7);
	glRasterPos3d(2.2, -2.45, 0.1);
	display_string("h", GLUT_BITMAP_HELVETICA_18);
	glPushMatrix();
	glColor3d(0.7,0.7,0.7);
	glTranslated(2.4,-2.45,0.1);
	glScalef(0.15,0.15,0.15);
	draw_triangle();
	glPopMatrix();
	glPushMatrix();
	glColor3d(0.7,0.7,0.7);
	glTranslated(2.16,-2.48,0.1);
	glScalef(0.2,0.2,0.2);
	draw_square_edge();
	glPopMatrix();

	glColor3f(0.7, 0.7, 0.7);
	glRasterPos3d(-2.3, -2.4, 0.1);
	display_string("g", GLUT_BITMAP_HELVETICA_18);
	glPushMatrix();
	glColor3d(0.7,0.7,0.7);
	glTranslated(-2.4,-2.45,0.1);
	glScalef(0.15,0.15,0.15);
	glRotatef(-180,0.0,1.0,0.0);
	draw_triangle();
	glPopMatrix();
	glPushMatrix();
	glColor3d(0.7,0.7,0.7);
	glTranslated(-2.33,-2.48,0.1);
	glScalef(0.2,0.2,0.2);
	draw_square_edge();
	glPopMatrix();
}

void page2_option1() {
  	GLdouble pointA[] = {-2.5, 2.0, 0.0};
  	GLdouble pointB[] = {2.5, 2.0, 0.0};
  	GLdouble pointC[] = {2.5, -2.0, 0.0};
  	GLdouble pointD[] = {-2.5, -2.0, 0.0};

	GLdouble pointa[] = {2.0, 1.5, 0.1};
  	GLdouble pointb[] = {-2.0, 1.5, 0.1};
  	GLdouble pointc[] = {-2.0, -1.5, 0.1};
  	GLdouble pointd[] = {2.0, -1.5, 0.1};

	glPushMatrix();
	if (arr[1] != nullptr && arr[1][0] == '1') {
        glColor3d(1.0, 1.0 ,0.8);
    }
	else if(arr[1] != nullptr && arr[1][0] == '2') {
        glColor3d(0.8, 0.8, 1.0);
    }
	else if(arr[1] != nullptr && arr[1][0] == '3') {
        glColor3d(0.8, 1.0, 1.0);
    }
	else if(arr[1] != nullptr && arr[1][0] == '4') {
        glColor3d(1.0, 0.8, 1.0);
    }
	glBegin(GL_POLYGON);
	glVertex3dv(pointA);
	glVertex3dv(pointB);
	glVertex3dv(pointC);
	glVertex3dv(pointD);
	glEnd();
	glPopMatrix();

	glPushMatrix();	
	glColor3d(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
  	glBindTexture(GL_TEXTURE_2D, g_TextureHandles[0]);
	glBegin(GL_POLYGON);
	glTexCoord2d(1.0, 0.0); 
    glVertex3dv(pointa);
    glTexCoord2d(0.0, 0.0); 
    glVertex3dv(pointb);
	glTexCoord2d(0.0, 1.0); 
    glVertex3dv(pointc);
    glTexCoord2d(1.0, 1.0); 
	glVertex3dv(pointd);
    glEnd();
  	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void page2_option2() {
  	GLdouble pointA[] = {-2.5, 2.0, 0.0};
  	GLdouble pointB[] = {2.5, 2.0, 0.0};
  	GLdouble pointC[] = {2.5, -2.0, 0.0};
  	GLdouble pointD[] = {-2.5, -2.0, 0.0};

	GLdouble pointa[] = {2.0, 1.5, 0.1};
  	GLdouble pointb[] = {-2.0, 1.5, 0.1};
  	GLdouble pointc[] = {-2.0, -1.5, 0.1};
  	GLdouble pointd[] = {2.0, -1.5, 0.1};

	glPushMatrix();
	if (arr[1] != nullptr && arr[1][0] == '1') {
        glColor3d(1.0, 1.0 ,0.8);
    }
	else if(arr[1] != nullptr && arr[1][0] == '2') {
        glColor3d(0.8, 0.8, 1.0);
    }
	else if(arr[1] != nullptr && arr[1][0] == '3') {
        glColor3d(0.8, 1.0, 1.0);
    }
	else if(arr[1] != nullptr && arr[1][0] == '4') {
        glColor3d(1.0, 0.8, 1.0);
    }
	glBegin(GL_POLYGON);
	glVertex3dv(pointA);
	glVertex3dv(pointB);
	glVertex3dv(pointC);
	glVertex3dv(pointD);
	glEnd();
	glPopMatrix();

	glPushMatrix();	
	glColor3d(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
  	glBindTexture(GL_TEXTURE_2D, g_TextureHandles[1]);
	glBegin(GL_POLYGON);
	glTexCoord2d(1.0, 0.0); 
    glVertex3dv(pointa);
    glTexCoord2d(0.0, 0.0); 
    glVertex3dv(pointb);
	glTexCoord2d(0.0, 1.0); 
    glVertex3dv(pointc);
    glTexCoord2d(1.0, 1.0); 
	glVertex3dv(pointd);
    glEnd();
  	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void page2_option3() {
  	GLdouble pointA[] = {-2.5, 2.0, 0.0};
  	GLdouble pointB[] = {2.5, 2.0, 0.0};
  	GLdouble pointC[] = {2.5, -2.0, 0.0};
  	GLdouble pointD[] = {-2.5, -2.0, 0.0};

	GLdouble pointa[] = {2.0, 1.5, 0.1};
  	GLdouble pointb[] = {-2.0, 1.5, 0.1};
  	GLdouble pointc[] = {-2.0, -1.5, 0.1};
  	GLdouble pointd[] = {2.0, -1.5, 0.1};

	glPushMatrix();
	if (arr[1] != nullptr && arr[1][0] == '1') {
        glColor3d(1.0, 1.0 ,0.8);
    }
	else if(arr[1] != nullptr && arr[1][0] == '2') {
        glColor3d(0.8, 0.8, 1.0);
    }
	else if(arr[1] != nullptr && arr[1][0] == '3') {
        glColor3d(0.8, 1.0, 1.0);
    }
	else if(arr[1] != nullptr && arr[1][0] == '4') {
        glColor3d(1.0, 0.8, 1.0);
    }
	glBegin(GL_POLYGON);
	glVertex3dv(pointA);
	glVertex3dv(pointB);
	glVertex3dv(pointC);
	glVertex3dv(pointD);
	glEnd();
	glPopMatrix();

	glPushMatrix();	
	glColor3d(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
  	glBindTexture(GL_TEXTURE_2D, g_TextureHandles[2]);
	glBegin(GL_POLYGON);
	glTexCoord2d(1.0, 0.0); 
    glVertex3dv(pointa);
    glTexCoord2d(0.0, 0.0); 
    glVertex3dv(pointb);
	glTexCoord2d(0.0, 1.0); 
    glVertex3dv(pointc);
    glTexCoord2d(1.0, 1.0); 
	glVertex3dv(pointd);
    glEnd();
  	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void page2_option4() {
  	GLdouble pointA[] = {-2.5, 2.0, 0.0};
  	GLdouble pointB[] = {2.5, 2.0, 0.0};
  	GLdouble pointC[] = {2.5, -2.0, 0.0};
  	GLdouble pointD[] = {-2.5, -2.0, 0.0};

	GLdouble pointa[] = {2.0, 1.5, 0.1};
  	GLdouble pointb[] = {-2.0, 1.5, 0.1};
  	GLdouble pointc[] = {-2.0, -1.5, 0.1};
  	GLdouble pointd[] = {2.0, -1.5, 0.1};

	glPushMatrix();
	if (arr[1] != nullptr && arr[1][0] == '1') {
        glColor3d(1.0, 1.0 ,0.8);
    }
	else if(arr[1] != nullptr && arr[1][0] == '2') {
        glColor3d(0.8, 0.8, 1.0);
    }
	else if(arr[1] != nullptr && arr[1][0] == '3') {
        glColor3d(0.8, 1.0, 1.0);
    }
	else if(arr[1] != nullptr && arr[1][0] == '4') {
        glColor3d(1.0, 0.8, 1.0);
    }
	glBegin(GL_POLYGON);
	glVertex3dv(pointA);
	glVertex3dv(pointB);
	glVertex3dv(pointC);
	glVertex3dv(pointD);
	glEnd();
	glPopMatrix();

	glPushMatrix();	
	glColor3d(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
  	glBindTexture(GL_TEXTURE_2D, g_TextureHandles[3]);
	glBegin(GL_POLYGON);
	glTexCoord2d(1.0, 0.0); 
    glVertex3dv(pointa);
    glTexCoord2d(0.0, 0.0); 
    glVertex3dv(pointb);
	glTexCoord2d(0.0, 1.0); 
    glVertex3dv(pointc);
    glTexCoord2d(1.0, 1.0); 
	glVertex3dv(pointd);
    glEnd();
  	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void page2_option5() {
  	GLdouble pointA[] = {-2.5, 2.0, 0.0};
  	GLdouble pointB[] = {2.5, 2.0, 0.0};
  	GLdouble pointC[] = {2.5, -2.0, 0.0};
  	GLdouble pointD[] = {-2.5, -2.0, 0.0};

	GLdouble pointa[] = {2.0, 1.5, 0.1};
  	GLdouble pointb[] = {-2.0, 1.5, 0.1};
  	GLdouble pointc[] = {-2.0, -1.5, 0.1};
  	GLdouble pointd[] = {2.0, -1.5, 0.1};

	glPushMatrix();
	if (arr[1] != nullptr && arr[1][0] == '1') {
        glColor3d(1.0, 1.0 ,0.8);
    }
	else if(arr[1] != nullptr && arr[1][0] == '2') {
        glColor3d(0.8, 0.8, 1.0);
    }
	else if(arr[1] != nullptr && arr[1][0] == '3') {
        glColor3d(0.8, 1.0, 1.0);
    }
	else if(arr[1] != nullptr && arr[1][0] == '4') {
        glColor3d(1.0, 0.8, 1.0);
    }
	glBegin(GL_POLYGON);
	glVertex3dv(pointA);
	glVertex3dv(pointB);
	glVertex3dv(pointC);
	glVertex3dv(pointD);
	glEnd();
	glPopMatrix();

	glPushMatrix();	
	glColor3d(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
  	glBindTexture(GL_TEXTURE_2D, g_TextureHandles[4]);
	glBegin(GL_POLYGON);
	glTexCoord2d(1.0, 0.0); 
    glVertex3dv(pointa);
    glTexCoord2d(0.0, 0.0); 
    glVertex3dv(pointb);
	glTexCoord2d(0.0, 1.0); 
    glVertex3dv(pointc);
    glTexCoord2d(1.0, 1.0); 
	glVertex3dv(pointd);
    glEnd();
  	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void page3_option1(){
    cv::Mat frame;
    cap >> frame;
    if(frame.empty()){
		cap.open(input_video);
		return; 
    }
    cv::cvtColor(frame,frame, cv::COLOR_BGR2RGB);
    if (frame.cols != TEXTURE_WIDTH || frame.rows != TEXTURE_HEIGHT) {
        cv::resize(frame, frame, cv::Size(TEXTURE_WIDTH, TEXTURE_HEIGHT));
    }
    glBindTexture(GL_TEXTURE_2D, g_TextureHandles[5]);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frame.cols, frame.rows, GL_RGB, GL_UNSIGNED_BYTE, frame.data);

	GLdouble pointA[] = {-2.5, 2.0, 0.0};
  	GLdouble pointB[] = {2.5, 2.0, 0.0};
  	GLdouble pointC[] = {2.5, -2.0, 0.0};
  	GLdouble pointD[] = {-2.5, -2.0, 0.0};

	GLdouble pointa[] = {2.0, 1.5, 0.1};
  	GLdouble pointb[] = {-2.0, 1.5, 0.1};
  	GLdouble pointc[] = {-2.0, -1.5, 0.1};
  	GLdouble pointd[] = {2.0, -1.5, 0.1};

	glPushMatrix();
	if (arr[1] != nullptr && arr[1][0] == '1') {
        glColor3d(1.0, 1.0 ,0.8);
    }
	else if(arr[1] != nullptr && arr[1][0] == '2') {
        glColor3d(0.8, 0.8, 1.0);
    }
	else if(arr[1] != nullptr && arr[1][0] == '3') {
        glColor3d(0.8, 1.0, 1.0);
    }
	else if(arr[1] != nullptr && arr[1][0] == '4') {
        glColor3d(1.0, 0.8, 1.0);
    }
	glBegin(GL_POLYGON);
	glVertex3dv(pointA);
	glVertex3dv(pointB);
	glVertex3dv(pointC);
	glVertex3dv(pointD);
	glEnd();
	glPopMatrix();
  	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, g_TextureHandles[5]);
	glBegin(GL_POLYGON);
  	glTexCoord2d(1.0, 0.0);
  	glVertex3dv(pointa);
  	glTexCoord2d(0.0, 0.0);
  	glVertex3dv(pointb);
  	glTexCoord2d(0.0, 1.0);
 	glVertex3dv(pointc);
  	glTexCoord2d(1.0, 1.0);
  	glVertex3dv(pointd);
  	glEnd();
  	glDisable(GL_TEXTURE_2D);

	display_fixed_elements();
}

void page4_option1(){
	static int page4_counter = 0;
	glPushMatrix();
	glRotated(30.0,1.0,0.0,0.0);
	if(page4_counter > 0){
		glPushMatrix();
    	glTranslatef(0.0f, -1.0f, 0.0f); 
    	glColor3f(1.0f, 0.5, 0.0); 
    	draw_cylinder(2.0f, 0.1f, 36); 
		glPopMatrix();

		glPushMatrix();
    	glTranslatef(0.0f, -1.5f, 0.0f); 
    	glColor3f(1.0f, 0.5, 0.0); 
    	draw_cylinder(0.5f, 0.6f, 36); 
		glPopMatrix();

		glPushMatrix();
    	glTranslatef(0.0f, -2.11f, 0.0f); 
    	glColor3f(1.0f, 0.5, 0.0); 
    	draw_cylinder(1.0f, 0.1f, 36); 
		glPopMatrix();
	}
	if(page4_counter>50){
		glPushMatrix();
    	glTranslatef(0.0f, -0.2f, 0.0f); 
    	glColor3f(1.0f, 1.0f, 1.0f); 
    	draw_cylinder(1.5f, 0.7f, 36); 
		glPopMatrix();
	}
	if(page4_counter>100){
		glPushMatrix();
  		glColor3d(1.0, 0.0, 0.0);
		glTranslated(0.0, 0.5, 1.0);
		glRotatef(-90,1.0,0.0,0.0);
		glutSolidCone(0.2,0.3,20,20);
  		glPopMatrix();

		glPushMatrix();
  		glColor3d(1.0, 0.0, 0.0);
		glTranslated(1.0, 0.5, 0.0);
		glRotatef(-90,1.0,0.0,0.0);
		glutSolidCone(0.2,0.3,20,20);
  		glPopMatrix();

		glPushMatrix();
  		glColor3d(1.0, 0.0, 0.0);
		glTranslated(-1.0, 0.5, 0.0);
		glRotatef(-90,1.0,0.0,0.0);
		glutSolidCone(0.2,0.3,20,20);
  		glPopMatrix();

  		glPushMatrix();
  		glColor3d(1.0, 0.0, 0.0);
		glTranslated(0.0, 0.5, -1.0);
		glRotatef(-90,1.0,0.0,0.0);
		glutSolidCone(0.2,0.3,20,20);
  		glPopMatrix();
  
		glPushMatrix();
  		glColor3d(1.0, 0.0, 0.0);
		glTranslated(0.75, 0.5, 0.75);
		glRotatef(-90,1.0,0.0,0.0);
		glutSolidCone(0.2,0.3,20,20);
  		glPopMatrix();

		glPushMatrix();
  		glColor3d(1.0, 0.0, 0.0);
		glTranslated(0.75, 0.5, -0.75);
		glRotatef(-90,1.0,0.0,0.0);
		glutSolidCone(0.2,0.3,20,20);
  		glPopMatrix();
  
  		glPushMatrix();
  		glColor3d(1.0, 0.0, 0.0);
		glTranslated(-0.75, 0.5, 0.75);
		glRotatef(-90,1.0,0.0,0.0);
		glutSolidCone(0.2,0.3,20,20);
  		glPopMatrix();
  
  		glPushMatrix();
  		glColor3d(1.0, 0.0, 0.0);
		glTranslated(-0.75, 0.5, -0.75);
		glRotatef(-90,1.0,0.0,0.0);
		glutSolidCone(0.2,0.3,20,20);
  		glPopMatrix();
	}
	if(page4_counter>150){
		glPushMatrix();
  		glColor3d(0.95, 0.85, 0.95);
		glTranslated(0.9, 0.5, 0.4);
		glRotatef(-90,1.0,0.0,0.0);
		glutSolidCone(0.2,0.3,20,20);
  		glPopMatrix();

		glPushMatrix();
  		glColor3d(0.95, 0.85, 0.95);
		glTranslated(0.9, 0.5, -0.4);
		glRotatef(-90,1.0,0.0,0.0);
		glutSolidCone(0.2,0.3,20,20);
  		glPopMatrix();

		glPushMatrix();
  		glColor3d(0.95, 0.85, 0.95);
		glTranslated(0.4, 0.5, 0.9);
		glRotatef(-90,1.0,0.0,0.0);
		glutSolidCone(0.2,0.3,20,20);
  		glPopMatrix();

  		glPushMatrix();
  		glColor3d(0.95, 0.85, 0.95);
		glTranslated(0.4, 0.5, -0.9);
		glRotatef(-90,1.0,0.0,0.0);
		glutSolidCone(0.2,0.3,20,20);
  		glPopMatrix();
  
		glPushMatrix();
  		glColor3d(0.95, 0.85, 0.95);
		glTranslated(-0.4, 0.5, -0.9);
		glRotatef(-90,1.0,0.0,0.0);
		glutSolidCone(0.2,0.3,20,20);
  		glPopMatrix();

		glPushMatrix();
  		glColor3d(0.95, 0.85, 0.95);
		glTranslated(-0.4, 0.5, 0.9);
		glRotatef(-90,1.0,0.0,0.0);
		glutSolidCone(0.2,0.3,20,20);
  		glPopMatrix();
  
  		glPushMatrix();
  		glColor3d(0.95, 0.85, 0.95);
		glTranslated(-0.9, 0.5, -0.4);
		glRotatef(-90,1.0,0.0,0.0);
		glutSolidCone(0.2,0.3,20,20);
  		glPopMatrix();
  
  		glPushMatrix();
  		glColor3d(0.95, 0.85, 0.95);
		glTranslated(-0.9, 0.5, 0.4);
		glRotatef(-90,1.0,0.0,0.0);
		glutSolidCone(0.2,0.3,20,20);
  		glPopMatrix();
	}
	if(page4_counter>200){
		glPushMatrix();
   	 	glTranslated(1.1, 0.5, 0.5);
    	glColor3f(1.0f, 1.0f, 0.0f); 
		draw_cylinder(0.05, 0.7, 36); 
		glPopMatrix();
		glPushMatrix();
  		glColor3d(1.0, 0.5, 0.0);
		glTranslated(1.1, 1.2, 0.5);
		glRotatef(-90,1.0,0.0,0.0);
		glutSolidCone(0.05,0.2,20,20);
  		glPopMatrix();

		glPushMatrix();
    	glTranslated(1.1, 0.5, -0.5);
    	glColor3f(1.0f, 1.0f, 0.0f); 
		draw_cylinder(0.05, 0.7, 36); 
		glPopMatrix();
		glPushMatrix();
  		glColor3d(1.0, 0.5, 0.0);
		glTranslated(1.1, 1.2, -0.5);
		glRotatef(-90,1.0,0.0,0.0);
		glutSolidCone(0.05,0.2,20,20);
  		glPopMatrix();

		glPushMatrix();
    	glTranslated(-1.1, 0.5, 0.5);
    	glColor3f(1.0f, 1.0f, 0.0f); 
   	 	draw_cylinder(0.05, 0.7, 36); 	
		glPopMatrix();
		glPushMatrix();
  		glColor3d(1.0, 0.5, 0.0);
		glTranslated(-1.1, 1.2, 0.5);
		glRotatef(-90,1.0,0.0,0.0);
		glutSolidCone(0.05,0.2,20,20);
  		glPopMatrix();

		glPushMatrix();
    	glTranslated(-1.1, 0.5, -0.5);
    	glColor3f(1.0f, 1.0f, 0.0f); 
    	draw_cylinder(0.05, 0.7, 36); 
		glPopMatrix();
		glPushMatrix();
  		glColor3d(1.0, 0.5, 0.0);
		glTranslated(-1.1, 1.2, -0.5);
		glRotatef(-90,1.0,0.0,0.0);
		glutSolidCone(0.05,0.2,20,20);
  		glPopMatrix();

		glPushMatrix();
   		glTranslated(0.5, 0.7, 1.1);
    	glColor3f(1.0f, 1.0f, 0.0f); 
    	draw_cylinder(0.05, 0.5, 36); 
		glPopMatrix();
		glPushMatrix();
  		glColor3d(1.0, 0.5, 0.0);
		glTranslated(0.5, 1.2, 1.1);
		glRotatef(-90,1.0,0.0,0.0);
		glutSolidCone(0.05,0.2,20,20);
  		glPopMatrix();

		glPushMatrix();
    	glTranslated(0.5, 0.7, -1.1);
    	glColor3f(1.0f, 1.0f, 0.0f); 
    	draw_cylinder(0.05, 0.5, 36); 
		glPopMatrix();
		glPushMatrix();
  		glColor3d(1.0, 0.5, 0.0);
		glTranslated(0.5, 1.2, -1.1);
		glRotatef(-90,1.0,0.0,0.0);
		glutSolidCone(0.05,0.2,20,20);
  		glPopMatrix();

		glPushMatrix();
    	glTranslated(-0.5, 0.7, 1.1);
    	glColor3f(1.0f, 1.0f, 0.0f); 
    	draw_cylinder(0.05, 0.5, 36); 
		glPopMatrix();
		glPushMatrix();
  		glColor3d(1.0, 0.5, 0.0);
		glTranslated(-0.5, 1.2, 1.1);
		glRotatef(-90,1.0,0.0,0.0);
		glutSolidCone(0.05,0.2,20,20);
  		glPopMatrix();

		glPushMatrix();
    	glTranslated(-0.5, 0.7, -1.1);
    	glColor3f(1.0f, 1.0f, 0.0f); 
    	draw_cylinder(0.05, 0.5, 36); 
		glPopMatrix();
		glPushMatrix();
  		glColor3d(1.0, 0.5, 0.0);
		glTranslated(-0.5, 1.2, -1.1);
		glRotatef(-90,1.0,0.0,0.0);
		glutSolidCone(0.05,0.2,20,20);
  		glPopMatrix();
	}
	if(page4_counter>250){
		glColor3d(0.6,0.0,0.2); 
		display_fixed_text_free("HAPPY ", 150, 440, GLUT_BITMAP_TIMES_ROMAN_24);
		display_fixed_text_free("BIRTHDAY", 250, 440, GLUT_BITMAP_TIMES_ROMAN_24);
	}
		glPopMatrix();
		page4_counter++;

	display_fixed_elements();
}

void page5_option1() {
	if(page5_num  == 1){
		glPushMatrix();
		glColor3d(0.0, 0.0, 0.0);
		glRotatef(-45.0, 1.0, 0.0, 0.0);  
		glRasterPos3d(-0.2, -0.15, 0.71); 
		display_string("Best", GLUT_BITMAP_HELVETICA_18);
		glPopMatrix();
		glColor3d(0.0,0.0,0.0);
		display_fixed_text_free("Best", 130, WINDOW_Y - 400, GLUT_BITMAP_HELVETICA_18);
	}
	else if(page5_num == 2){
		glColor3d(0.0,0.0,0.0);
		display_fixed_text_free("Best", 130, WINDOW_Y - 400, GLUT_BITMAP_HELVETICA_18);
	}
	else if(page5_num  == 3){
		glPushMatrix();
		glColor3d(0.0, 0.0, 0.0);
		glRotatef(45.0, 0.0, 1.0, 0.0); 
		glTranslatef(-0.1, 0.0, 0.7);
		glRasterPos3d(0.0, 0.0, 0.05); 
		display_string("Wishes", GLUT_BITMAP_HELVETICA_18);
		glPopMatrix();
		glColor3d(0.0,0.0,0.0);
		display_fixed_text_free("Best wishes", 130, WINDOW_Y - 400, GLUT_BITMAP_HELVETICA_18);
	}
	else if(page5_num == 4){
		glColor3d(0.0,0.0,0.0);
		display_fixed_text_free("Best wishes", 130, WINDOW_Y - 400, GLUT_BITMAP_HELVETICA_18);
	}
	else if(page5_num  == 5){
		glPushMatrix();
		glColor3d(0.0, 0.0, 0.0);
		glRotatef(-45.0, 0.0, 1.0, 0.0); 
		glRotatef(90.0, 1.0, 0.0, 0.0); 
		glRasterPos3d(-0.35, 0.15, 0.65); 
		display_string("for", GLUT_BITMAP_HELVETICA_18);
		glPopMatrix();
		glColor3d(0.0,0.0,0.0);
		display_fixed_text_free("Best wishes for", 130, WINDOW_Y - 400, GLUT_BITMAP_HELVETICA_18);
	}
	else if(page5_num == 6){
		glColor3d(0.0,0.0,0.0);
		display_fixed_text_free("Best wishes for", 130, WINDOW_Y - 400, GLUT_BITMAP_HELVETICA_18);
	}
	else if(page5_num == 7){
		glPushMatrix();
		glColor3d(0.0, 0.0, 0.0);
		glRotatef(-50.0, 0.0, 1.0, 0.0); 
		glRotatef(-30.0, 0.0, 0.0, 1.0); 
		glRasterPos3d(-0.6,-0.5,0.1); 
		display_string("your", GLUT_BITMAP_HELVETICA_18);
		glPopMatrix();
		glColor3d(0.0,0.0,0.0);
		display_fixed_text_free("Best wishes for your", 130, WINDOW_Y - 400, GLUT_BITMAP_HELVETICA_18);
	}
	else if(page5_num == 8){
		glColor3d(0.0,0.0,0.0);
		display_fixed_text_free("Best wishes for your", 130, WINDOW_Y - 400, GLUT_BITMAP_HELVETICA_18);
	}
	else if(page5_num == 9){
		glPushMatrix();
		glColor3d(0.0, 0.0, 0.0);	
		glRasterPos3d(0.3,0.4,-0.6); 
		display_string("birthday", GLUT_BITMAP_HELVETICA_18);
		glPopMatrix();
		glColor3d(0.0,0.0,0.0);
		display_fixed_text_free("Best wishes for your birthday.", 130, WINDOW_Y - 400, GLUT_BITMAP_HELVETICA_18);
	}
	else if(page5_num == 10){
		glColor3d(0.0,0.0,0.0);
		display_fixed_text_free("Best wishes for your birthday.", 130, WINDOW_Y - 400, GLUT_BITMAP_HELVETICA_18);
	}
	else if(page5_num == 11){
		glPushMatrix();
		glColor3d(0.0, 0.0, 0.0);
		glRotatef(-55.0, 0.0, 0.0, 1.0); 
		glRasterPos3d(-0.7,0.0,0.0); 
		display_string("I", GLUT_BITMAP_HELVETICA_18);
		glPopMatrix();
		glColor3d(0.0,0.0,0.0);
		display_fixed_text_free("Best wishes for your birthday.", 130, WINDOW_Y - 400, GLUT_BITMAP_HELVETICA_18);
		display_fixed_text_free("I", 130, WINDOW_Y - 430, GLUT_BITMAP_HELVETICA_18);
	}
	else if(page5_num == 12){
		glColor3d(0.0,0.0,0.0);
		display_fixed_text_free("Best wishes for your birthday.", 130, WINDOW_Y - 400, GLUT_BITMAP_HELVETICA_18);
		display_fixed_text_free("I", 130, WINDOW_Y - 430, GLUT_BITMAP_HELVETICA_18);
	}
	else if(page5_num == 13){
		glPushMatrix();
		glColor3d(0.0, 0.0, 0.0);
		glRotatef(45.0, 0.0, 1.0, 0.0); 
		glRotatef(90.0, 1.0, 0.0, 0.0); 
		glRasterPos3d(0.1, 0.4, 0.7); 
		display_string("hope", GLUT_BITMAP_HELVETICA_18);
		glPopMatrix();
		glColor3d(0.0,0.0,0.0);
		display_fixed_text_free("Best wishes for your birthday.", 130, WINDOW_Y - 400, GLUT_BITMAP_HELVETICA_18);
		display_fixed_text_free("I hope", 130, WINDOW_Y - 430, GLUT_BITMAP_HELVETICA_18);
	}
	else if(page5_num == 14){
		glColor3d(0.0,0.0,0.0);
		display_fixed_text_free("Best wishes for your birthday.", 130, WINDOW_Y - 400, GLUT_BITMAP_HELVETICA_18);
		display_fixed_text_free("I hope", 130, WINDOW_Y - 430, GLUT_BITMAP_HELVETICA_18);
	}
 	else if(page5_num == 15){
		glPushMatrix();
		glColor3d(0.0, 0.0, 0.0);
		glRotatef(-45.0, 0.0, 1.0, 0.0); 
		glTranslatef(-0.3, 0.0, 0.7);
		glRasterPos3d(0.0, 0.0, 0.05); 
		display_string("you", GLUT_BITMAP_HELVETICA_18);
		glPopMatrix();
		glColor3d(0.0,0.0,0.0);
		display_fixed_text_free("Best wishes for your birthday.", 130, WINDOW_Y - 400, GLUT_BITMAP_HELVETICA_18);
		display_fixed_text_free("I hope you", 130, WINDOW_Y - 430, GLUT_BITMAP_HELVETICA_18);
	}
	else if(page5_num == 16){
		glColor3d(0.0,0.0,0.0);
		display_fixed_text_free("Best wishes for your birthday.", 130, WINDOW_Y - 400, GLUT_BITMAP_HELVETICA_18);
		display_fixed_text_free("I hope you", 130, WINDOW_Y - 430, GLUT_BITMAP_HELVETICA_18);
	}
	else if(page5_num == 17){
		glPushMatrix();
		glColor3d(0.0, 0.0, 0.0);
		glRotatef(45.0, 0.0, 1.0, 0.0); 
		glRasterPos3d(0.8,0.0,0.4); 
		display_string("have", GLUT_BITMAP_HELVETICA_18);
		glPopMatrix();
		glColor3d(0.0,0.0,0.0);
		display_fixed_text_free("Best wishes for your birthday.", 130, WINDOW_Y - 400, GLUT_BITMAP_HELVETICA_18);
		display_fixed_text_free("I hope you have", 130, WINDOW_Y - 430, GLUT_BITMAP_HELVETICA_18);
	}
	else if(page5_num == 18){
		glColor3d(0.0,0.0,0.0);
		display_fixed_text_free("Best wishes for your birthday.", 130, WINDOW_Y - 400, GLUT_BITMAP_HELVETICA_18);
		display_fixed_text_free("I hope you have", 130, WINDOW_Y - 430, GLUT_BITMAP_HELVETICA_18);
	}
	else if(page5_num == 19){
		glPushMatrix();
		glColor3d(0.0, 0.0, 0.0);
		glRotatef(45.0, 1.0, 0.0, 0.0); 
		glRasterPos3d(0.0, 0.1, 0.73); 
		display_string("a", GLUT_BITMAP_HELVETICA_18);
		glPopMatrix();
		glColor3d(0.0,0.0,0.0);
		display_fixed_text_free("Best wishes for your birthday.", 130, WINDOW_Y - 400, GLUT_BITMAP_HELVETICA_18);
		display_fixed_text_free("I hope you have a", 130, WINDOW_Y - 430, GLUT_BITMAP_HELVETICA_18);
	}
	else if(page5_num == 20){
		glColor3d(0.0,0.0,0.0);
		display_fixed_text_free("Best wishes for your birthday.", 130, WINDOW_Y - 400, GLUT_BITMAP_HELVETICA_18);
		display_fixed_text_free("I hope you have a", 130, WINDOW_Y - 430, GLUT_BITMAP_HELVETICA_18);
	}
	else if(page5_num == 21){
		glPushMatrix();
		glColor3d(0.0, 0.0, 0.0);
		glRasterPos3d(0.2,-0.5,-0.6); 
		display_string("nice", GLUT_BITMAP_HELVETICA_18);
		glPopMatrix();
		glColor3d(0.0,0.0,0.0);
		display_fixed_text_free("Best wishes for your birthday.", 130, WINDOW_Y - 400, GLUT_BITMAP_HELVETICA_18);
		display_fixed_text_free("I hope you have a nice", 130, WINDOW_Y - 430, GLUT_BITMAP_HELVETICA_18);
	}
	else if(page5_num == 22){
		glColor3d(0.0,0.0,0.0);
		display_fixed_text_free("Best wishes for your birthday.", 130, WINDOW_Y - 400, GLUT_BITMAP_HELVETICA_18);
		display_fixed_text_free("I hope you have a nice", 130, WINDOW_Y - 430, GLUT_BITMAP_HELVETICA_18);
	}
	else if(page5_num == 23){
		glPushMatrix();
		glColor3d(0.0, 0.0, 0.0);
		glRotatef(45.0, 0.0, 0.0, 1.0); 
		glRasterPos3d(0.7,0.1,0.2); 
		display_string("year", GLUT_BITMAP_HELVETICA_18);
		glPopMatrix();
		glColor3d(0.0,0.0,0.0);
		display_fixed_text_free("Best wishes for your birthday.", 130, WINDOW_Y - 400, GLUT_BITMAP_HELVETICA_18);
		display_fixed_text_free("I hope you have a nice year.", 130, WINDOW_Y - 430, GLUT_BITMAP_HELVETICA_18);
	}

	display_fixed_elements();

	glPushMatrix();
	glScalef(0.5,0.5,0.5);
	drawDodecahedron();
	glPopMatrix();

    glPushMatrix();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, WINDOW_X, 0, WINDOW_Y, -1, 1); 

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    GLdouble pointa[] = {-0, 500, 0}; 
    GLdouble pointb[] = {150, 500, 0};
    GLdouble pointc[] = {150, 350, 0}; 
    GLdouble pointd[] = {0, 350, 0}; 

    glEnable(GL_TEXTURE_2D);
	int text_num = page5_num/2;
    glBindTexture(GL_TEXTURE_2D, g_TextureHandles2[text_num]);
	glColor3d(1.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
    glBegin(GL_POLYGON);
	glTexCoord2d(0.0, 0.0); glVertex3dv(pointa);  
	glTexCoord2d(1.0, 0.0); glVertex3dv(pointb);  
	glTexCoord2d(1.0, 1.0); glVertex3dv(pointc);  
	glTexCoord2d(0.0, 1.0); glVertex3dv(pointd);  
	glEnd();

    glEnd();

    glDisable(GL_TEXTURE_2D);

    glPopMatrix(); 
    glMatrixMode(GL_PROJECTION);
    glPopMatrix(); 
    glMatrixMode(GL_MODELVIEW);

    glPopMatrix();

	glColor3d(0.0, 0.0, 0.0);
	display_fixed_text_free("1. Place the object", 170, 460, GLUT_BITMAP_9_BY_15);
	display_fixed_text_free("   as shown in the left picture.", 170, 440, GLUT_BITMAP_9_BY_15);
	display_fixed_text_free("2. Push n and check the word.", 170, 420, GLUT_BITMAP_9_BY_15);
	display_fixed_text_free("3. Push n and you can see", 170, 400, GLUT_BITMAP_9_BY_15);
	display_fixed_text_free("   the next picture.", 170, 380, GLUT_BITMAP_9_BY_15);
	display_fixed_text_free("* Push b to go back one step.", 170, 360, GLUT_BITMAP_9_BY_15);
}

void page6_option0(){
	glPushMatrix();
	glColor3f(1.0, 0.0, 0.5);
	display_fixed_text_free("HAPPY", 50, 100, GLUT_BITMAP_TIMES_ROMAN_24);
	display_fixed_text_free("BIRTHDAY", 160, 100, GLUT_BITMAP_TIMES_ROMAN_24);
	display_fixed_text_free("TO", 310, 100, GLUT_BITMAP_TIMES_ROMAN_24);
	display_fixed_text_free("YOU", 370, 100, GLUT_BITMAP_TIMES_ROMAN_24);
	display_fixed_text_free("!", 430, 100, GLUT_BITMAP_TIMES_ROMAN_24);
	glPopMatrix();

	static int page5_counter = 0;

    if (page5_counter > 50) {
        glColor3f(0.0, 0.0, 0.0);
        glRasterPos3d(-2.1, 1.0, 0.0);
        display_string("To", GLUT_BITMAP_TIMES_ROMAN_24);
    }
    if (page5_counter >= 100) {
        glColor3f(0.0, 0.0, 0.0);
        glRasterPos3d(-1.6, 1.0, 0.0);
        display_string(arr[2], GLUT_BITMAP_TIMES_ROMAN_24);
    }	
	if (page5_counter >= 150) {
        glColor3f(0.0, 0.0, 0.0);
        glRasterPos3d(-1.6, 0.2, 0.0);
        display_string("On", GLUT_BITMAP_TIMES_ROMAN_24);
    }
	if (page5_counter >= 200) {
        glColor3f(0.0, 0.0, 0.0);
        glRasterPos3d(-1.0, 0.2, 0.0);
        display_string("your", GLUT_BITMAP_TIMES_ROMAN_24);
    }
	if (page5_counter >= 250) {
        glColor3f(0.0, 0.0, 0.0);
        glRasterPos3d(-0.3, 0.2, 0.0);
        display_string("special", GLUT_BITMAP_TIMES_ROMAN_24);
    }
	if (page5_counter >= 300) {
        glColor3f(0.0, 0.0, 0.0);
        glRasterPos3d(0.7, 0.2, 0.0);
        display_string("day", GLUT_BITMAP_TIMES_ROMAN_24);
    }
	if (page5_counter >= 350) {
        glColor3f(0.0, 0.0, 0.0);
        glRasterPos3d(1.3, 0.2, 0.0);
        display_string("...", GLUT_BITMAP_TIMES_ROMAN_24);
    }
	if (page5_counter >= 400) {
        glColor3f(0.0, 0.0, 0.0);
        glRasterPos3d(0.0, -0.6, 0.0);
        display_string("From", GLUT_BITMAP_TIMES_ROMAN_24);
    }
	if (page5_counter >= 450) {
        glColor3f(0.0, 0.0, 0.0);
        glRasterPos3d(0.8, -0.6, 0.0);
        display_string(arr[3], GLUT_BITMAP_TIMES_ROMAN_24);
    }

    page5_counter++;

	glPushMatrix();
	glColor3d(0.6,1.0,0.8); 
   	glTranslated(1.0, 1.0, 0.0);
	glScalef(0.6,0.6,0.6);
	glRotatef(-40,0.0,0.0,1.0);
	draw_ballon();
  	glPopMatrix();

	glPushMatrix();
	glColor3d(1.0, 0.8, 0.8); 
   	glTranslated(1.0, 1.0, 0.0);
	glScalef(0.6,0.6,0.6);
	glRotatef(40,0.0,0.0,1.0);
	glRotatef(15,1.0,0.0,0.0);
	draw_ballon();
  	glPopMatrix();

	glPushMatrix();
	glColor3d(1.0, 1.0, 0.6); 
   	glTranslated(1.0, 1.0, 0.0);
	glScalef(0.6,0.6,0.6);
	glRotatef(-5,0.0,0.0,1.0);
	draw_ballon();
  	glPopMatrix();

	glPushMatrix();
	glColor3d(0.8, 1.0, 0.6); 
   	glTranslated(1.0, 1.0, 0.0);
	glScalef(0.6,0.6,0.6);
	glRotatef(-25,1.0,0.0,1.0);
	glRotatef(-25,1.0,0.0,0.0);
	draw_ballon();
  	glPopMatrix();

	glPushMatrix();
	glColor3d(0.8, 0.6, 0.8); 
   	glTranslated(1.0, 1.0, 0.0);
	glScalef(0.6,0.6,0.6);
	glRotatef(25,0.0,0.0,1.0);
	glRotatef(-30,1.0,0.0,0.0);
	draw_ballon();
  	glPopMatrix();

	glPushMatrix();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, WINDOW_X, 0, WINDOW_Y, -1, 1); 

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(0.7, 0.7, 0.7);
    glRasterPos2f(44, WINDOW_Y - 480);
    display_string("g", GLUT_BITMAP_HELVETICA_18);
 
    glPushMatrix();
    glColor3d(0.7, 0.7, 0.7);
    glTranslated(30, WINDOW_Y - 485, 0); 
    glScalef(20, 20, 1); 
    glRotatef(-180, 0.0, 1.0, 0.0); 
    draw_triangle();
    glPopMatrix();
    glPushMatrix();
    glColor3d(0.7, 0.7, 0.7);
    glTranslated(40, WINDOW_Y - 488, 0); 
    glScalef(25, 25, 1);
    draw_square_edge();
    glPopMatrix();

    glPopMatrix(); 
    glMatrixMode(GL_PROJECTION);
    glPopMatrix(); 
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void set_texture(){
   	const char *inputFileNames[5] = {arr[5], arr[6],arr[7], arr[8], arr[9]};
   	const char *inputFileNames2[12] = {"page5/best.png","page5/wishes.png","page5/for.png", "page5/your.png","page5/birthday.png","page5/i.png","page5/hope.png","page5/you.png", "page5/have.png","page5/another.png", "page5/nice.png", "page5/year.png",};
  	for(int i = 0; i < 5; i++){
    	cv::Mat input = cv::imread(inputFileNames[i], 1);
  
    	cv::cvtColor(input, input, cv::COLOR_BGR2RGB);
		int newWidth, newHeight;
		float aspectRatio = (float)input.cols / (float)input.rows;

		if (TEXTURE_WIDTH / TEXTURE_HEIGHT > aspectRatio) {
    		newHeight = TEXTURE_HEIGHT;
    		newWidth = (int)(TEXTURE_HEIGHT * aspectRatio);
		} 
		else {
    		newWidth = TEXTURE_WIDTH;
   	 		newHeight = (int)(TEXTURE_WIDTH / aspectRatio);
		}
	cv::resize(input, input, cv::Size(newWidth, newHeight));
	int top = (TEXTURE_HEIGHT - newHeight) / 2;
	int bottom = TEXTURE_HEIGHT - newHeight - top;
	int left = (TEXTURE_WIDTH - newWidth) / 2;
	int right = TEXTURE_WIDTH - newWidth - left;

	cv::copyMakeBorder(input, input, top, bottom, left, right, cv::BORDER_CONSTANT, cv::Scalar(0, 0, 0)); // 黒い余白

    glBindTexture(GL_TEXTURE_2D, g_TextureHandles[i]);
    glTexSubImage2D(GL_TEXTURE_2D, 0,
        (TEXTURE_WIDTH - input.cols)/2,
        (TEXTURE_HEIGHT - input.rows)/2,
        input.cols, input.rows,
        GL_RGB, GL_UNSIGNED_BYTE, input.data);
  	}    
  	for(int i = 0; i < 12; i++){
    	cv::Mat input = cv::imread(inputFileNames2[i], 1);

    	cv::cvtColor(input, input, cv::COLOR_BGR2RGB);

    	glBindTexture(GL_TEXTURE_2D, g_TextureHandles2[i]);
    	glTexSubImage2D(GL_TEXTURE_2D, 0,
        	(TEXTURE_WIDTH - input.cols)/2,
        	(TEXTURE_HEIGHT - input.rows)/2,
        	input.cols, input.rows,
        	GL_RGB, GL_UNSIGNED_BYTE, input.data);
  	}    
}

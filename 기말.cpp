#include <cstdlib>
#include "Foothold.h"
#include"filetobuf.h"
#include "Robot.h"
#define pi 3.141592


GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid KeyboardUp(unsigned char key, int x, int y);
//void Mouse(int button, int state, int x, int y);
//void MouseMove(GLint x, GLint y);
void Timerfunction(int value);
void make_vertexShader();
void make_fragmentShader();

void check_collide();
bool collide_box(Foothold, Robot&);

using namespace std;

int g_window_w;
int g_window_h;

GLuint vao, vbo[2];
GLuint s_program;

GLchar* vertexsource, * fragmentsource; // 소스코드 저장 변수
GLuint vertexshader, fragmentshader; // 세이더 객체
float theta = 0;
float cx = 1, cy = 1, cz = 1;

using namespace std;

GLfloat	box[][3] = {
	{ -0.5, 0, 0.5 },
	{ 0.5, 0, 0.5 },
	{ 0.5,1, 0.5 },	

	{ 0.5, 1, 0.5 },
	{ -0.5, 1, 0.5 },
	{ -0.5, 0, 0.5 }, // 앞면

	{ -0.5, 0, -0.5 },
	{ 0.5, 0, -0.5 },
	{ -0.5, 0, 0.5 },

	{ 0.5, 0, -0.5 },
	{ 0.5, 0, 0.5 },
	{ -0.5, 0, 0.5 }, // 밑면

	{ -0.5,0, 0.5 }, 
	{ -0.5, 1, 0.5 },
	{ -0.5, 0, -0.5 },

	{ -0.5, 1, 0.5 },
	{ -0.5, 1, -0.5 },
	{ -0.5, 0, -0.5 }, //왼옆면 (바라보는기준)

	{ -0.5, 0, -0.5 }, 
	{ 0.5, 0, -0.5 },
	{ -0.5, 1, -0.5 },

	{ 0.5, 0, -0.5 },
	{ 0.5, 1, -0.5 },
	{ -0.5, 1, -0.5 }, //뒷면

	{ -0.5, 1, 0.5 },
	{ 0.5, 1, 0.5 },
	{ -0.5, 1, -0.5 },

	{ -0.5, 1, -0.5 },
	{ 0.5, 1, 0.5 },
	{ 0.5,1, -0.5 }, //윗면

	{ 0.5, 1, -0.5 },
	{ 0.5, 1, 0.5 },
	{ 0.5,0, -0.5 },

	{ 0.5, 1, 0.5 },
	{ 0.5, 0, 0.5 },
	{ 0.5, 0, -0.5 }, //오른옆면
};
GLfloat	boxN[][3] = {
	 0.0f,  0.0f,  1.0f, 
	 0.0f,  0.0f,  1.0f,
	 0.0f,  0.0f,  1.0f,

	 0.0f,  0.0f,  1.0f,
	 0.0f,  0.0f,  1.0f,
	 0.0f,  0.0f,  1.0f, // 앞면

	  0.0f, -1.0f,  0.0f,
	  0.0f, -1.0f,  0.0f,
	  0.0f, -1.0f,  0.0f,

	  0.0f, -1.0f,  0.0f,
	  0.0f, -1.0f,  0.0f,
	  0.0f, -1.0f,  0.0f, // 밑면

	  -1.0f,  0.0f,  0.0f,
	  -1.0f,  0.0f,  0.0f,
	  -1.0f,  0.0f,  0.0f,

	  -1.0f,  0.0f,  0.0f,
	  -1.0f,  0.0f,  0.0f,
	  -1.0f,  0.0f,  0.0f, //옆면

	  0.0f,  0.0f, -1.0f,
	  0.0f,  0.0f, -1.0f,
	  0.0f,  0.0f, -1.0f,

	  0.0f,  0.0f, -1.0f,
	  0.0f,  0.0f, -1.0f,
	  0.0f,  0.0f, -1.0f, //뒷면

	  0.0f,  1.0f,  0.0f,
	  0.0f,  1.0f,  0.0f,
	  0.0f,  1.0f,  0.0f,

	  0.0f,  1.0f,  0.0f,
	  0.0f,  1.0f,  0.0f,
	  0.0f,  1.0f,  0.0f, //윗면

	  1.0f,  0.0f,  0.0f,
	  1.0f,  0.0f,  0.0f,
	  1.0f,  0.0f,  0.0f,

	  1.0f,  0.0f,  0.0f,
	  1.0f,  0.0f,  0.0f,
	  1.0f,  0.0f,  0.0f,
};

std::vector<Foothold> Bottom;
Robot player;

void init()
{
	glEnable(GL_DEPTH_TEST);
}

void make_vertexShader()
{
	vertexsource = filetobuf("vertex.glsl");
	//--- 버텍스 세이더 객체 만들기
	vertexshader = glCreateShader(GL_VERTEX_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(vertexshader, 1, (const GLchar**)&vertexsource, 0);
	//--- 버텍스 세이더 컴파일하기
	glCompileShader(vertexshader);
	//--- 컴파일이 제대로 되지 않은 경우: 에러 체크
	checkCompileErrors(vertexshader, "vertex shader");
}

void make_fragmentShader()
{
	fragmentsource = filetobuf("fragment.glsl");
	//--- 프래그먼트 세이더 객체 만들기
	fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(fragmentshader, 1, (const GLchar**)&fragmentsource, 0);
	//--- 프래그먼트 세이더 컴파일
	glCompileShader(fragmentshader);
	//--- 컴파일이 제대로 되지 않은 경우: 컴파일 에러 체크
	checkCompileErrors(fragmentshader, "fragment shader");
}

void InitBuffer()
{
	glGenVertexArrays(1, &vao); //--- VAO 를 지정하고 할당하기
	glGenBuffers(2, vbo); //--- 2개의 VBO를 지정하고 할당하기

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(box), box, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boxN), boxN, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
}

void InitShader()
{
	make_vertexShader();
	make_fragmentShader();
	//-- shader Program
	s_program = glCreateProgram();
	glAttachShader(s_program, vertexshader);
	glAttachShader(s_program, fragmentshader);
	glLinkProgram(s_program);
	checkCompileErrors(s_program, "PROGRAM");
	//--- 세이더 삭제하기
	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);
	//--- Shader Program 사용하기
	glUseProgram(s_program);
}

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	srand(time(0));

	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA|GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(600, 600);
	glutCreateWindow("기말");
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();
	InitShader();
	InitBuffer();

	init();
	MakeFoothold(Bottom);

	player.y = 5;
	player.fall = true;
	player.Locate();

	glutTimerFunc(100, Timerfunction, 1);

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(KeyboardUp);
	glutMainLoop();
}

void Timerfunction(int value)
{
	switch (value) {
	case 1:
		player.Update();
		check_collide();

		for (int i = 0; i < Bottom.size(); ++i) {
			if (Bottom[i].startDel)
				Bottom[i].Delete();
		}

		for (int i = 0; i < Bottom.size(); ++i) {
			if (Bottom[i].Del)
				Bottom.erase(Bottom.begin() + i);
		}

		glutPostRedisplay();
		glutTimerFunc(100, Timerfunction, 1);
		break;
	}
}

float radius = 5, camX=0,camY=0,camZ=0;
float lx = 3.0, ly = 2.0, lz = -1.5, ltheta=0; 
float Lx = 0, Ly = 0, Lz = 0;
float aml = 0.3f;

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	//--- 변경된 배경색 설정
	glClearColor(0.2, 0.2, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// 렌더링 파이프라인에 세이더 불러오기

	glUseProgram(s_program);

	GLUquadricObj * armL, * armR;
	armL = gluNewQuadric();
	armR = gluNewQuadric();

	glm::mat4 ptrans = glm::mat4(1.0f);
	glm::mat4 vtrans = glm::mat4(1.0f);

	Lx = (float)cos(ltheta / 180 * 3.141592) * lx;			// 조명 회전
	Ly = ly;
	Lz = (float)sin(ltheta / 180 * 3.141592) * (-lz);

	unsigned int lightPosLocation = glGetUniformLocation(s_program, "lightPos"); //--- lightPos 값 전달: (0.0, 0.0, 5.0);
	glUniform3f(lightPosLocation, Lx,Ly,Lz);
	unsigned int lightColorLocation = glGetUniformLocation(s_program, "lightColor"); //--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
	glUniform3f(lightColorLocation, cx, cy, cz);
	unsigned int amlight = glGetUniformLocation(s_program, "ambientLight");
	glUniform1f(amlight, aml);
	

	unsigned int color_location = glGetUniformLocation(s_program, "objectColor");
	unsigned int model = glGetUniformLocation(s_program, "model");

	camX = (float)sin(theta / 180 * 3.141592) * radius;
	camY = +0.0;
	camZ = -1 * (float)cos(theta / 180 * 3.141592) * radius;
	//vtrans = glm::lookAt(glm::vec3(camX,camY,camZ), glm::vec3(X,Y,Z-5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	vtrans = glm::lookAt(glm::vec3(0, player.y + 3, player.z + 3), glm::vec3(camX, player.y, player.z), glm::vec3(0.0f, 1.0f, 0.0f));

	unsigned int view = glGetUniformLocation(s_program, "view");
	glUniformMatrix4fv(view, 1, GL_FALSE, &vtrans[0][0]);

	unsigned int projection = glGetUniformLocation(s_program, "projection");
	ptrans = glm::perspective(glm::radians(45.0f), (float)g_window_w / (float)g_window_h, 0.1f, 100.0f);
	ptrans = glm::translate(ptrans, glm::vec3(0, 0, -5.0));

	glUniformMatrix4fv(projection, 1, GL_FALSE, &ptrans[0][0]);

	glBindVertexArray(vao);
	for (int i = 0; i < Bottom.size(); ++i)
	{
		Bottom[i].Draw_Start();
		glUniform3f(color_location, Bottom[i].r, Bottom[i].g, Bottom[i].b);
		glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(glm::mat4(Bottom[i].Drawing)));
		glDrawArrays(GL_TRIANGLES, 0, Bottom[i].size);
	}

	// head
	glUniform3f(color_location, player.head.r, player.head.g, player.head.b);
	glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(glm::mat4(player.head.TRS)));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	// nose
	glUniform3f(color_location, player.nose.r, player.nose.g, player.nose.b);
	glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(glm::mat4(player.nose.TRS)));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	// body
	glUniform3f(color_location, player.body.r, player.body.g, player.body.b);
	glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(glm::mat4(player.body.TRS)));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	// arm
	glUniform3f(color_location, player.arm_l.r, player.arm_l.g, player.arm_l.b);
	glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(glm::mat4(player.arm_l.TRS)));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glUniform3f(color_location, player.arm_r.r, player.arm_r.g, player.arm_r.b);
	glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(glm::mat4(player.arm_r.TRS)));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	// leg
	glUniform3f(color_location, player.leg_l.r, player.leg_l.g, player.leg_l.b);
	glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(glm::mat4(player.leg_l.TRS)));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glUniform3f(color_location, player.leg_r.r, player.leg_r.g, player.leg_r.b);
	glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(glm::mat4(player.leg_r.TRS)));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	g_window_w = w;
	g_window_h = h;
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
	{
		player.dz = -0.1;
		player.angle_turn = 180;
		break;
	}
	case 'a':
	{
		player.dx = -0.1;
		player.angle_turn = -90;
		break;
	}
	case 's':
	{
		player.dz = 0.1;
		player.angle_turn = 0;
		break;
	}
	case 'd':
	{
		player.dx = 0.1;
		player.angle_turn = 90;
		break;
	}
	case 32:
	{
		player.Jump();
		break;
	}
	case 'p':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 'P':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 'Q':
		exit(0);
		break;
	}
	InitBuffer();
	glutPostRedisplay();
}

GLvoid KeyboardUp(unsigned char key, int x, int y)
{
	switch (key){
	case 'w': case 's':
	{
		player.dz = 0;
		break;
	}
	case 'a': case'd':
	{
		player.dx = 0;
		break;
	}
	}
	glutPostRedisplay();
}

void check_collide()
{
	player.fall = true;
	for (int i = 0; i < Bottom.size(); ++i) {
		if (collide_box(Bottom[i], player)) {
			player.fall = false;
			player.dy = 0;
			Bottom[i].startDel = true;
			break;
		}
	}

	for (int i = 0; i < Bottom.size(); ++i) {
		if (Bottom[i].Del)
			Bottom.erase(Bottom.begin() + i);
	}
}

bool collide_box(Foothold bottom, Robot& player)
{
	float b_maxX, b_minX, p_maxX, p_minX;
	float b_maxY, b_minY, p_maxY, p_minY;
	float b_maxZ, b_minZ, p_maxZ, p_minZ;
	p_maxX = player.x + 0.15; p_minX = player.x - 0.15;
	p_maxY = player.y + 0.1; p_minY = player.y - 0.1;
	p_maxZ = player.z + 0.15; p_minZ = player.z - 0.15;

	b_maxX = bottom.mx + 0.4; b_minX = bottom.mx - 0.4;
	b_maxY = bottom.my + 0.35; b_minY = bottom.my + 0.25;
	b_maxZ = bottom.mz + 0.4; b_minZ = bottom.mz - 0.4;

	if (b_maxX < p_minX || b_minX > p_maxX)
		return false;  
	if (b_maxZ < p_minZ || b_minZ > p_maxZ)
		return false;
	if (b_maxY < p_minY || b_minY > p_maxY)
		return false;
	player.y = bottom.my + 0.3;
	return true;
}
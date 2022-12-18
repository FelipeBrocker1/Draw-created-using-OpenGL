#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define _USE_MATH_DEFINES
#include <math.h>

class Circle
{
private:
	GLint vertexCount;
	GLfloat* circleVertex;
	GLfloat* circleColor;
	GLfloat radius;
	GLfloat x, y;
	GLuint sides;
	GLuint VAO;
	GLuint VBO;
	GLuint color_VBO;

public:
	Circle(GLfloat radius, GLfloat x, GLfloat y, GLuint sides)
	{
		this->radius = radius;
		this->x = x;
		this->y = y;
		this->sides = sides;
	}

	void generateCircle()
	{
		vertexCount = sides + 2;

		GLfloat twoPi = 2.0f * M_PI;

		GLfloat* circleVertexX = new GLfloat[vertexCount];
		GLfloat* circleVertexY = new GLfloat[vertexCount];
		circleColor = new GLfloat[vertexCount*3];

		//0.980, 0.719, 0.00
		//Generating colors.
		for (int i = 0; i < vertexCount; i++)
		{
			circleColor[i*3] = 0.980;
			circleColor[(i*3) + 1] = 0.719;
			circleColor[(i*3) + 2] = 0.0;

			std::cout << "\nR: " << circleColor[i * 3] << "   Indice: " << (i*3);
			std::cout << "\nG: " << circleColor[(i * 3) + 1] << "   Indice: " << (i*3)+1;
			std::cout << "\nB: " << circleColor[(i * 3) + 2] << "   Indice: " << (i+3)+2;

		}

		//Central point
		circleVertexX[0] = x;
		circleVertexY[0] = y;

		//Generating vertex positions:
		for (int i = 1; i < vertexCount; i++)
		{
			circleVertexX[i] = x + (radius * cos(i * twoPi / sides));
			circleVertexY[i] = y + (radius * sin(i * twoPi / sides));
		}

		//In the buffer we only pass one vertex.
		circleVertex = new GLfloat[vertexCount * 2]; //Multiply the vertex numbers by 2 because of the vertex arrays' XY.

		for (int i = 0; i < vertexCount; i++)
		{
			circleVertex[(i * 2)] = circleVertexX[i];
			circleVertex[(i * 2) + 1] = circleVertexY[i];

			std::cout << "X: " << circleVertex[(i * 2)] << std::endl;
			std::cout << "Y: " << circleVertex[(i * 2) + 1] << std::endl;
		}
		delete[] circleVertexX;
		delete[] circleVertexY;
	}

	//This function will create the VBO and the VAO of my obj.
	void createBuffer()
	{

		glGenBuffers(1, &VBO); //Generates an array on the GPU.
		glBindBuffer(GL_ARRAY_BUFFER, VBO); //The OpenGL is a state machine, it'll render always the selected object.
		glBufferData(GL_ARRAY_BUFFER, 2 * vertexCount * sizeof(GLfloat), circleVertex, GL_STATIC_DRAW); //Placing the circleVertex data inside my VBO.

		glGenBuffers(2, &color_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, color_VBO);
		glBufferData(GL_ARRAY_BUFFER, 3 * vertexCount * sizeof(GLfloat), circleColor, GL_STATIC_DRAW);

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 2, (GLvoid*)0);
			
		glBindBuffer(GL_ARRAY_BUFFER, color_VBO);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 3, (GLvoid*)0);
				
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
	}

	void drawCircle()
	{
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);
	}

	GLuint getCurrentVAO()
	{
		return VAO;
	}

	GLint getVertexCount()
	{
		return vertexCount;
	}

	void liberateMemory()
	{
		circleVertex = nullptr;
		circleColor = nullptr;
		delete[] circleVertex;
		delete[] circleColor;
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}
};

void createBuffer(GLuint& position_VBO, GLuint& color_VBO, GLuint& VAO, GLfloat* positionArray, GLfloat* colorArray, GLint colorSize, GLint positionSize)
{
	glGenBuffers(1, &position_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, position_VBO);
	glBufferData(GL_ARRAY_BUFFER, positionSize * sizeof(float), positionArray, GL_STATIC_DRAW);
	positionArray = nullptr;

	glGenBuffers(1, &color_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, color_VBO);
	glBufferData(GL_ARRAY_BUFFER, colorSize * sizeof(float), colorArray, GL_STATIC_DRAW);
	colorArray = nullptr;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, position_VBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, color_VBO);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}

int main()
{
	glfwInit(); //Inicializando GLFW.

	GLFWwindow* janela = glfwCreateWindow(800, 800, "Felipe Brocker Dornelles", NULL, NULL);

	if (!janela)
	{
		std::cout << "\n Falha ao abrir janela" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(janela); //Especifico uma instancia de janela.
	gladLoadGL(); //Habilita OpenGl.

	//Criando os shaders:
	const GLchar* vertexShaderSource =
		"#version 330 core\n"
		"layout (location = 0) in vec3 vertex_position;\n"
		"layout (location = 1) in vec3 vertex_color;\n"
		"out vec3 frag_color;\n"
		"void main()\n"
		"{\n"
		"	frag_color = vertex_color;\n"
		"   gl_Position = vec4(vertex_position, 1.0);\n"
		"}\0";

	const GLchar* fragmentShaderSource =
		"#version 330\n"
		"out vec4 color;\n"
		"in vec3 frag_color;\n"
		"void main()\n"
		"{\n"
		"	color = vec4(frag_color, 1.0);\n"
		"}\n\0";

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); //Vertex shader criado.

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);  //Fragment shader criado.

	//Compilando os shaders: GL_TRUE(compilado sem erros) e GL_FALSE(Compilado com erros)
	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);

	//Criando um programa e associando os shaders a ele:
	GLuint programa = glCreateProgram();
	glAttachShader(programa, vertexShader);
	glAttachShader(programa, fragmentShader);

	//Linking program to use on GPU:
	glLinkProgram(programa);

	//Linked objects can be deleted:
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	GLfloat grass[] =
	{
		//Grass
		1.0f,  -0.8f, //Down right C 
		1.0f,  -0.7f, //Top right B 
		-1.0f, -0.7f, //Top left A 
		- 1.0f, -0.8f //Down left D 
	};
	GLfloat grassColors[] =
	{
		0.0305f, 0.610f, 0.00f,
		0.472f, 1.00f, 0.120f,
		0.472f, 1.00f, 0.120f,
		0.0305f, 0.610f, 0.00f,
	};
	GLuint pGrass_VBO = 0;
	GLuint cGrass_VBO = 1;
	GLuint Grass_VAO = 1;
	createBuffer(pGrass_VBO, cGrass_VBO, Grass_VAO, grass, grassColors, 12, 8);

	GLfloat dirt[] =
	{
		1.0f, -1.0f, //Down right F
		1.0f, -0.8f, //Top right C
		-1.0f, -0.8f, //Top left D
		-1.0f, -1.0f //Down left E
	};
	GLfloat dirtColors[] =
	{
		0.260f, 0.147f, 0.00f,
		0.490f, 0.278f, 0.00f,
		0.490f, 0.278f, 0.00f,
		0.260f, 0.147f, 0.00f
	};
	GLuint pDirt_VBO = 0;
	GLuint cDirt_VBO = 1;
	GLuint Dirt_VAO = 2;
	createBuffer(pDirt_VBO, cDirt_VBO, Dirt_VAO, dirt, dirtColors, 12, 8);

	GLfloat house[] =
	{
		1.0f, -0.7f, //Down right B
		1.0f, 0.3f,  //Top right M
		0.1f, 0.3f, //Top left L
		0.1f, -0.7f //Down left K
	};
	GLfloat houseColors[] =
	{
		0.610f, 0.557f, 0.384f,
		0.800f, 0.729f, 0.496f,
		0.800f, 0.729f, 0.496f,
		0.610f, 0.557f, 0.384f
	};
	GLuint pHouse_VBO = 0;
	GLuint cHouse_VBO = 1;
	GLuint House_VAO = 3;
	createBuffer(pHouse_VBO, cHouse_VBO, House_VAO, house, houseColors, 12, 8);
	
	Circle sun(0.172f, 0.0f, 0.8f, 60);
	sun.generateCircle();
	sun.createBuffer();

	GLfloat roof[] =
	{
		0.55f, 0.85f,
		0.1f, 0.3f,
		1.0f, 0.3f
	};
	GLfloat roofColors[] =
	{
		1.0f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f,
	};
	GLuint pRoof_VBO = 0;
	GLuint cRoof_VBO = 1;
	GLuint Roof_VAO = 4;
	createBuffer(pRoof_VBO, cRoof_VBO, Roof_VAO, roof, roofColors, 9, 6);

	GLfloat head[] =
	{
		-0.3, -0.05, //Down right T
		-0.3, 0.15, //Top right H1
		-0.5, 0.15, //Top left G1
		-0.5, -0.05 //Down left S

	};
	GLfloat headColors[] =
	{
		0.790f, 0.593f, 0.395f,
		0.830f, 0.606f, 0.382f,
		0.830f, 0.606f, 0.382f,
		0.790f, 0.593f, 0.395f
	};
	GLuint pHead_VBO = 0;
	GLuint cHead_VBO = 1;
	GLuint Head_VAO = 5;
	createBuffer(pHead_VBO, cHead_VBO, Head_VAO, head, headColors, sizeof(headColors), sizeof(head));

	GLfloat tshirt[] =
	{
		-0.3f, -0.28f, //Top right A  
		-0.3f, -0.05f, //Top right T
		-0.5f, -0.05f, //Top left G1
		-0.5f, -0.28f //Top left D
	};
	GLfloat tshirtColors[]
	{
		0.00530f, 0.530f, 0.443f,
		0.00f, 0.874f, 0.990f,
		0.00f, 0.874f, 0.990f,
		0.00530f, 0.530f, 0.443f
	};
	GLuint pTshirt_VBO = 0;
	GLuint cTshirt_VBO = 1;
	GLuint Tshirt_VAO = 6;
	createBuffer(pTshirt_VBO, cTshirt_VBO, Tshirt_VAO, tshirt, tshirtColors, sizeof(tshirtColors), sizeof(tshirt));
	
	GLfloat pants[] =
	{
		-0.5f, -0.28f, //Top left D
		-0.3f, -0.28f, //Top right A  
		-0.3f, -0.65f,
		-0.5f, -0.65f
	};
	GLfloat pantsColor[] =
	{
		0.310f, 0.176f, 0.980f,
		0.310f, 0.176f, 0.980f,
		0.204f, 0.143f, 0.510f,
		0.204f, 0.143f, 0.510
	};
	GLuint pPants_VBO = 0;
	GLuint cPants_VBO = 1;
	GLuint Pants_VAO = 7;
	createBuffer(pPants_VBO, cPants_VBO, Pants_VAO, pants, pantsColor, sizeof(pantsColor), sizeof(pants));

	GLfloat shortSleeve[] =
	{
		-0.5f, -0.12f, //Down right
		-0.5f, -0.05f, //Top right C
		-0.6f, -0.05f, //Top left I
		-0.6f, -0.12f //Down left K
	};
	GLuint pShortSleeve_VBO = 0;
	GLuint ShortSleeve_VAO = 8;
	createBuffer(pShortSleeve_VBO, cTshirt_VBO, ShortSleeve_VAO, shortSleeve, tshirtColors, sizeof(tshirtColors), sizeof(shortSleeve));

	GLfloat shortSleeve2[] =
	{
		-0.3f, -0.12f, //Down right
		-0.3f, -0.05f, //Top right C
		-0.2f, -0.05f, //Top left I
		-0.2f, -0.12f //Down left K
	};
	GLuint pShortSleeve_VBO2 = 0;
	GLuint ShortSleeve_VAO2 = 9;
	createBuffer(pShortSleeve_VBO2, cTshirt_VBO, ShortSleeve_VAO2, shortSleeve2, tshirtColors, sizeof(tshirtColors), sizeof(shortSleeve2));

	GLfloat arm1[]
	{
		-0.5f, -0.37f, //Down right N
		-0.5f, -0.12f, //Top right L
		-0.6f, -0.12f, //Top left K
		-0.6f, -0.37f,  //Down left M
	};
	GLuint pArm_VBO = 0;
	GLuint arm_VAO = 10;
	createBuffer(pArm_VBO, cHead_VBO, arm_VAO, arm1, headColors, sizeof(headColors), sizeof(arm1));

	GLfloat arm2[]
	{
		-0.2f, -0.37f, //Down right N
		-0.2f, -0.12f, //Top right L
		-0.3f, -0.12f, //Top left K
		-0.3f, -0.37f,  //Down left M
	};
	GLuint pArm_VBO2 = 0;
	GLuint arm_VAO2 = 11;
	createBuffer(pArm_VBO2, cHead_VBO, arm_VAO2, arm2, headColors, sizeof(headColors), sizeof(arm2));

	GLfloat hair[]
	{
		-0.3f, 0.15f,
		-0.3f, 0.2f,
		-0.5f, 0.2f,
		-0.5f, 0.15f
	};
	GLfloat hairColor[] =
	{
		0.320f, 0.137f, 0.00640f,
		0.320f, 0.137f, 0.00640f,
		0.320f, 0.137f, 0.00640f,
		0.320f, 0.137f, 0.00640f
	};
	GLuint pHair_VBO = 0;
	GLuint cHair_VBO = 1;
	GLuint hair_VAO = 12;
	createBuffer(pHair_VBO, cHair_VBO, hair_VAO, hair, hairColor, sizeof(hairColor), sizeof(hair));

	GLfloat hair2[]
	{
		-0.5f, 0.15f,
		-0.47f, 0.15f,
		-0.47f, 0.12f,
		-0.5f, 0.12f
	};
	GLuint pHair2_VBO = 0;
	GLuint hair2_VAO = 13;
	createBuffer(pHair2_VBO, cHair_VBO, hair2_VAO, hair2, hairColor, sizeof(hairColor), sizeof(hair2));

	GLfloat hair3[]
	{
		-0.3f, 0.12f,
		-0.3f, 0.15f,
		-0.33f, 0.15f,
		-0.33f, 0.12f
	};
	GLuint pHair3_VBO = 0;
	GLuint hair3_VAO = 14;
	createBuffer(pHair3_VBO, cHair_VBO, hair3_VAO, hair3, hairColor, sizeof(hairColor), sizeof(hair3));
	
	GLfloat eye1[]
	{
		-0.42f, 0.05f, //J
		-0.42f, 0.08f, //I1
		-0.45f, 0.08f, //H1
		-0.45f, 0.05f //G1
	};
	GLuint eyeIris_VBO1 = 0;
	GLuint eyeIris_VAO1 = 15;
	createBuffer(eyeIris_VBO1, cPants_VBO, eyeIris_VAO1, eye1, pantsColor, sizeof(pantsColor), sizeof(eye1));

	GLfloat eye2[]
	{
		-0.38f, 0.05f, //P1
		-0.38f, 0.08f, //O1
		-0.35f, 0.08f, //N1
		-0.35f, 0.05f, //M1
	};
	GLuint eyeIris_VBO2 = 0;
	GLuint eyeIris_VAO2 = 16;
	createBuffer(eyeIris_VBO2, cPants_VBO, eyeIris_VAO2, eye2, pantsColor, sizeof(pantsColor), sizeof(eye2));

	GLfloat sclera[]
	{
		-0.45f, 0.05f,
		-0.45f, 0.08f,
		-0.48f, 0.08f,
		-0.48f, 0.05f
	};
	GLfloat scleraColor[]
	{
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f
	};

	GLuint sclera_VBO = 0;
	GLuint scleraColor_VBO = 1;
	GLuint sclera_VAO = 17;
	createBuffer(sclera_VBO, scleraColor_VBO, sclera_VAO, sclera, scleraColor, sizeof(scleraColor), sizeof(sclera));

	GLfloat sclera2[]
	{
		-0.32f, 0.05f, //L
		-0.32f, 0.08f, //K1
		-0.35f, 0.08f, //N1
		-0.35f, 0.05f //M1
	};
	GLuint sclera2_VBO = 0;
	GLuint sclera2_VAO = 18;
	createBuffer(sclera2_VBO, scleraColor_VBO, sclera2_VAO, sclera2, scleraColor, sizeof(scleraColor), sizeof(sclera2));

	GLfloat noose[]
	{
		-0.42f, 0.05f, //J1
		-0.38f, 0.05f, //P1
		-0.38f, 0.02f, //R1
		-0.42f, 0.02f //Q1
	};
	GLfloat nooseColor[]
	{
		0.430f, 0.280f, 0.172f,
		0.430f, 0.280f, 0.172f,
		0.430f, 0.280f, 0.172f,
		0.430f, 0.280f, 0.172f
	};
	GLuint noose_VBO = 0;
	GLuint nooseColor_VBO = 1;
	GLuint noose_VAO = 19;
	createBuffer(noose_VBO, nooseColor_VBO, noose_VAO, noose, nooseColor, sizeof(nooseColor), sizeof(noose));
	
	GLfloat mouth[]
	{
		-0.35f, -0.005f, //V1
		-0.35f, 0.02f, //T1
		-0.38f, 0.02f, //R1
		-0.38f, -0.005f //W1
	};
	GLfloat mouthColor[]
	{
		0.280f, 0.202f, 0.146f,
		0.280f, 0.202f, 0.146f,
		0.280f, 0.202f, 0.146f,
		0.280f, 0.202f, 0.146f
	};
	GLuint mouth_VBO = 0;
	GLuint mouthColor_VBO = 1;
	GLuint mouth_VAO = 20;
	createBuffer(mouth_VBO, mouthColor_VBO, mouth_VAO, mouth, mouthColor, sizeof(mouthColor), sizeof(mouth));
	
	GLfloat mouth2[]
	{
		-0.42f, -0.005f, //Z1
		-0.42f, 0.02f, //Q1
		-0.45f, 0.02f, //S1
		-0.45f, -0.005f //U1
	}; 
	GLuint mouth2_VBO = 0;
	GLuint mouth2_VAO = 21;
	createBuffer(mouth2_VBO, mouthColor_VBO, mouth2_VAO, mouth2, mouthColor, sizeof(mouthColor), sizeof(mouth2));
	
	GLfloat mouth3[]
	{
		-0.35f, -0.03f, //B2
		-0.35f, -0.005f, //V1
		-0.45f, -0.005f, //U1
		-0.45f, -0.03f //A2
	};
	GLuint mouth3_VBO = 0;
	GLuint mouth3_VAO = 22;
	createBuffer(mouth3_VBO, mouthColor_VBO, mouth3_VAO, mouth3, mouthColor, sizeof(mouthColor), sizeof(mouth3));

	GLfloat sneakers[]
	{
		-0.3f, -0.7f, //H
		-0.3f, -0.65f, //F
		-0.5f, -0.65f, //E
		-0.5f, -0.7f //G
	};
	GLfloat sneakersColor[]
	{
		0.280f, 0.265f, 0.255f,
		0.440f, 0.437f, 0.436f,
		0.440f, 0.437f, 0.436f,
		0.280f, 0.265f, 0.255f
	};
	GLuint sneakers_VBO = 0;
	GLuint sneakersColor_VBO = 1;
	GLuint sneakers_VAO = 23;
	createBuffer(sneakers_VBO, sneakersColor_VBO, sneakers_VAO, sneakers, sneakersColor, sizeof(sneakersColor), sizeof(sneakers));

	//Posicionando buffers no primeiro indice
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	GLint color = glGetUniformLocation(programa, "inputColor");

	glClearColor(0.0f, 0.6f, 0.8f, 0.0f);
	while (!glfwWindowShouldClose(janela)) //Esta funcao retorna o valor da flag de fechamento da janela especificada.
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(programa);

		glBindVertexArray(Grass_VAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glBindVertexArray(Dirt_VAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glBindVertexArray(House_VAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glBindVertexArray(House_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(Roof_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(Head_VAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glBindVertexArray(Tshirt_VAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glBindVertexArray(Pants_VAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glBindVertexArray(ShortSleeve_VAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glBindVertexArray(ShortSleeve_VAO2);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glBindVertexArray(arm_VAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glBindVertexArray(arm_VAO2);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glBindVertexArray(hair_VAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glBindVertexArray(hair2_VAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glBindVertexArray(hair3_VAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glBindVertexArray(eyeIris_VAO1);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glBindVertexArray(eyeIris_VAO2);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glBindVertexArray(sclera_VAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glBindVertexArray(sclera2_VAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		
		glBindVertexArray(noose_VAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glBindVertexArray(mouth_VAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		
		glBindVertexArray(mouth2_VAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);		
			
		glBindVertexArray(mouth3_VAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		
		glBindVertexArray(sneakers_VAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		sun.drawCircle();

		glfwSwapBuffers(janela); //Troca o back buffer para o front buffer.	
		glfwPollEvents();
	}

	//Liberando quaisquers recursos alocados durante ou apos a inicializacao.
	glDeleteProgram(programa);
	
	glDeleteBuffers(1, &pGrass_VBO);
	glDeleteBuffers(2, &cGrass_VBO);
	glDeleteVertexArrays(1, &Grass_VAO);

	glDeleteBuffers(1, &pDirt_VBO);
	glDeleteBuffers(2, &cDirt_VBO);
	glDeleteVertexArrays(2, &Dirt_VAO);

	glDeleteBuffers(1, &pHouse_VBO);
	glDeleteBuffers(2, &cHouse_VBO);
	glDeleteVertexArrays(3, &House_VAO);

	glDeleteBuffers(1, &pRoof_VBO);
	glDeleteBuffers(2, &cRoof_VBO);
	glDeleteVertexArrays(4, &Roof_VAO);

	glDeleteBuffers(1, &pHead_VBO);
	glDeleteBuffers(2, &cHead_VBO);
	glDeleteVertexArrays(5, &Head_VAO);

	glDeleteBuffers(1, &pHead_VBO);
	glDeleteBuffers(2, &cHead_VBO);
	glDeleteVertexArrays(6, &Head_VAO);

	glDeleteBuffers(1, &pTshirt_VBO);
	glDeleteBuffers(2, &cTshirt_VBO);
	glDeleteVertexArrays(7, &Tshirt_VAO);

	glDeleteBuffers(1, &pPants_VBO);
	glDeleteBuffers(2, &cPants_VBO);
	glDeleteVertexArrays(8, &Pants_VAO);

	glDeleteBuffers(1, &pShortSleeve_VBO);
	glDeleteBuffers(2, &pShortSleeve_VBO);
	glDeleteVertexArrays(9, &ShortSleeve_VAO);

	glDeleteBuffers(1, &pShortSleeve_VBO2);
	glDeleteVertexArrays(10, &ShortSleeve_VAO2);
	
	glDeleteBuffers(1, &pArm_VBO);
	glDeleteVertexArrays(11, &pArm_VBO);
	
	glDeleteBuffers(1, &pArm_VBO2);
	glDeleteVertexArrays(12, &pArm_VBO2);

	glDeleteBuffers(1, &pHair_VBO);
	glDeleteVertexArrays(13, &hair_VAO);
	
	glDeleteBuffers(1, &pHair2_VBO);
	glDeleteVertexArrays(14, &hair2_VAO);
	
	glDeleteBuffers(1, &pHair3_VBO);
	glDeleteVertexArrays(15, &hair3_VAO);

	glDeleteBuffers(1, &eyeIris_VAO1);
	glDeleteVertexArrays(16, &eyeIris_VAO1);

	glDeleteBuffers(1, &eyeIris_VAO2);
	glDeleteVertexArrays(17, &eyeIris_VAO2);

	glDeleteBuffers(1, &sclera_VBO);
	glDeleteBuffers(2, &scleraColor_VBO);
	glDeleteVertexArrays(18, &sclera_VAO);

	glDeleteBuffers(1, &sclera2_VBO);
	glDeleteVertexArrays(19, &sclera2_VAO);

	glDeleteBuffers(1, &noose_VBO);
	glDeleteBuffers(2, &nooseColor_VBO);
	glDeleteVertexArrays(20, &noose_VAO);

	glDeleteBuffers(1, &mouth_VBO);
	glDeleteBuffers(2, &mouthColor_VBO);
	glDeleteVertexArrays(21, &mouth_VAO);
	
	glDeleteBuffers(1, &mouth2_VBO);
	glDeleteVertexArrays(22, &mouth2_VAO);

	glDeleteBuffers(1, &mouth3_VBO);
	glDeleteVertexArrays(23, &mouth3_VAO);

	glDeleteBuffers(1, &sneakers_VBO);
	glDeleteBuffers(2, &sneakersColor_VBO);
	glDeleteVertexArrays(24, &sneakers_VAO);

	sun.liberateMemory();
	glfwDestroyWindow(janela);
	glfwTerminate();

	return 0;
}
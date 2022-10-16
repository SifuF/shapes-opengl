#ifndef SHAPE_H
#define SHAPE_H

#include<cmath>
#include <limits>

#include"VAO.hpp"
#include"VBO.hpp"
#include"EBO.hpp"
#include"Texture.hpp"

class Shape {
public:
	GLfloat* data;
	GLuint* indices;
	int size;
	int sizeInd;
	int points;
	VAO vao;
	VBO vbo;
	EBO ebo;
	int primitive;
	int attribCount;
	//Texture *texture;

	Shape() : data(nullptr), indices(nullptr), size(0), sizeInd(0), points(0), primitive(GL_TRIANGLES), attribCount(11) {}

	void drawShapeArrays(int prim) {
		vao.bind();
		vbo.bind();
		ebo.bind();
		glDrawArrays(prim, 0, points);
		vbo.unBind();
		vao.unBind();
		ebo.unBind();
	};

	void drawShapeElements(int prim) {
		vao.bind();
		vbo.bind();
		ebo.bind();
		glDrawElements(prim, points, GL_UNSIGNED_INT, 0);
		vbo.unBind();
		vao.unBind();
		ebo.unBind();
	};

	void layout() {
		vao.bind();
		vbo.bind();
		glBufferData(GL_ARRAY_BUFFER, 4 * size, data, GL_STATIC_DRAW);
		ebo.bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeInd, indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, attribCount * sizeof(float), (void*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, attribCount * sizeof(float), (void*)(3 * sizeof(float)));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, attribCount * sizeof(float), (void*)(6 * sizeof(float)));
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, attribCount * sizeof(float), (void*)(8 * sizeof(float)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);

		vbo.unBind();
		vao.unBind();
		ebo.unBind();

		// Bind the EBO to 0 so that we don't accidentally modify it
		// MAKE SURE TO UNBIND IT AFTER UNBINDING THE VAO, as the EBO is linked in the VAO
		// This does not apply to the VBO because the VBO is already linked to the VAO during glVertexAttribPointer
	}

	~Shape() {
		
	}

};

class CoordinateAxes : public Shape {
public:
	CoordinateAxes() {
		
		GLfloat r = 10.0f;
		size = attribCount * 4;
		sizeInd = 6;
		points = sizeInd;							//	2  
													//	| 3
		data = new GLfloat[size];					//	|/	
		GLfloat verts[] = {							//	0-----1
			0.0f, 0.0f, 0.0f,	1.0f, 1.0f, 1.0f,		0.0f, 1.0f,		0.0f, 0.0f, 1.0f,
			r, 0.0f, 0.0f,		1.0f, 0.0f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 1.0f,
			0.0f, r, 0.0f,		0.0f, 1.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, r,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		0.0f, 0.0f, 1.0f,
		};
		for (int i = 0; i < size; i++) {
			data[i] = verts[i];
		}

		indices = new GLuint[sizeInd];
		GLuint index[] = {
			0, 1,
			0, 2,
			0, 3
		};
		for (int i = 0; i < sizeInd; i++) {
			indices[i] = index[i];
		}

		layout();
	}

	void draw() {
		drawShapeElements(GL_LINES);
	}

	~CoordinateAxes() {
		delete[] indices;
		delete[] data;
	}

};


class Rectangle : public Shape {
public:

	Rectangle(GLfloat height, GLfloat width) {
		
		size = attribCount * 4;
		sizeInd = 6;			
		points = sizeInd;
													//	0-------2
		data = new GLfloat[size];					//	|		|
		GLfloat verts[] = {							//	1-------3
			-width / 2.0f, height / 2.0f, 0.0f,		1.0f, 0.0f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 1.0f,
			-width / 2.0f, -height / 2.0f, 0.0f,	0.0f, 1.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 1.0f,
			width / 2.0f, height / 2.0f, 0.0f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		0.0f, 0.0f, 1.0f,
			width / 2.0f, -height / 2.0f, 0.0f,		1.0f, 1.0f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 1.0f
		};
		for (int i = 0; i < size; i++) {
			data[i] = verts[i];
		}

		indices = new GLuint[sizeInd];
		GLuint index[] = {
			0, 1, 2,
			1, 3, 2
		};
		for (int i = 0; i < sizeInd; i++) {
			indices[i] = index[i];
		}	

		layout();
	}

	void draw() {
		drawShapeElements(primitive);
	}

	~Rectangle() {
		delete[] indices;
		delete[] data;
	}

};

class Cuboid : public Shape {
public:

	Cuboid( GLfloat width, GLfloat height, GLfloat depth) {//  16       18 
													//		8 4--------6 14
 		size = attribCount * 24;								//		 /|       /|
		sizeInd = 6*6;								//	 10 0--------2 | 12
		points = sizeInd;							//		| |      | |
													//	   9| 5------|-7 15		
		data = new GLfloat[size];					//		|/		 |/
		GLfloat verts[] = {							//	 11	1--------3 13
			-width / 2.0f, height / 2.0f, depth / 2.0f,		1.0f, 0.0f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 1.0f,	//front 0
			-width / 2.0f, -height / 2.0f, depth / 2.0f,	0.0f, 1.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 1.0f,
			width / 2.0f, height / 2.0f, depth / 2.0f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		0.0f, 0.0f, 1.0f,
			width / 2.0f, -height / 2.0f, depth / 2.0f,		1.0f, 1.0f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 1.0f,

			-width / 2.0f, height / 2.0f, -depth / 2.0f,	1.0f, 0.0f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, -1.0f,	//back 4
			-width / 2.0f, -height / 2.0f, -depth / 2.0f,	0.0f, 1.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, -1.0f,
			width / 2.0f, height / 2.0f, -depth / 2.0f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		0.0f, 0.0f, -1.0f,
			width / 2.0f, -height / 2.0f, -depth / 2.0f,	1.0f, 1.0f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, -1.0f,

			-width / 2.0f, height / 2.0f, -depth / 2.0f,	1.0f, 0.0f, 0.0f,		0.0f, 1.0f,		-1.0f, 0.0f, 0.0f,	//left 8
			-width / 2.0f, -height / 2.0f, -depth / 2.0f,	0.0f, 1.0f, 0.0f,		0.0f, 0.0f,		-1.0f, 0.0f, 0.0f,
			-width / 2.0f, height / 2.0f, depth / 2.0f,		1.0f, 0.0f, 0.0f,		1.0f, 1.0f,		-1.0f, 0.0f, 0.0f,
			-width / 2.0f, -height / 2.0f, depth / 2.0f,	0.0f, 1.0f, 0.0f,		1.0f, 0.0f,		-1.0f, 0.0f, 0.0f,

			width / 2.0f, height / 2.0f, depth / 2.0f,		0.0f, 0.0f, 1.0f,		0.0f, 1.0f,		1.0f, 0.0f, 0.0f,	//right 12
			width / 2.0f, -height / 2.0f, depth / 2.0f,		1.0f, 1.0f, 0.0f,		0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
			width / 2.0f, height / 2.0f, -depth / 2.0f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		1.0f, 0.0f, 0.0f,
			width / 2.0f, -height / 2.0f, -depth / 2.0f,	1.0f, 1.0f, 0.0f,		1.0f, 0.0f,		1.0f, 0.0f, 0.0f,

			-width / 2.0f, height / 2.0f, -depth / 2.0f,	1.0f, 0.0f, 0.0f,		0.0f, 1.0f,		0.0f, 1.0f, 0.0f,	//top 16
			-width / 2.0f, height / 2.0f, depth / 2.0f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
			width / 2.0f, height / 2.0f, -depth / 2.0f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		0.0f, 1.0f, 0.0f,
			width / 2.0f, height / 2.0f, depth / 2.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f,

			-width / 2.0f, -height / 2.0f, -depth / 2.0f,	0.0f, 1.0f, 0.0f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,	//bottom 20
			-width / 2.0f, -height / 2.0f, depth / 2.0f,	0.0f, 1.0f, 0.0f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
			width / 2.0f, -height / 2.0f, -depth / 2.0f,	1.0f, 1.0f, 0.0f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
			width / 2.0f, -height / 2.0f, depth / 2.0f,		1.0f, 1.0f, 0.0f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f


		};
		for (int i = 0; i < size; i++) {
			data[i] = verts[i];
		}

		indices = new GLuint[sizeInd];
		GLuint index[] = {
			0, 1, 2,
			1, 3, 2,

			6, 7, 5,
			5, 4, 6,

			8, 9, 11,
			11, 10, 8,

			12, 13, 15,
			15, 14, 12,

			16, 17, 18,
			17, 19, 18,

			20, 21, 22,
			21, 23, 22
		};
		for (int i = 0; i < sizeInd; i++) {
			indices[i] = index[i];
		}

		layout();
	}

	void draw() {
		drawShapeElements(primitive);
	}

	~Cuboid() {
		delete[] indices;
		delete[] data;
	}

};

class Circle : public Shape {
public:

	Circle(int n) {
		
		

		//each vert = 3pos + 3col + 2tex + 3 norm = 11
		//n steps requires n+1 verts including middle
		size = attribCount * (n + 1);
		data = new GLfloat[size];

		float r = 1.0f;
		float pi = 3.14159f;

		data[0] = 0.0f;
		data[1] = 0.0f;
		data[2] = 0.0f;

		data[3] = 0.0f;
		data[4] = 1.0f;
		data[5] = 0.0f;

		data[6] = 0.5f;
		data[7] = 0.5f;

		data[8] = 0.0f;
		data[9] = 0.0f;
		data[10] = -1.0f;

		int j = 0;
		for (int i = attribCount; i < size; i += attribCount) {
			data[i] = r * cos(j * 2 * pi / n);
			data[i + 1] = r * sin(j * 2 * pi / n);
			data[i + 2] = 0.0f;

			data[i + 3] = rand() / (1.0 * RAND_MAX);
			data[i + 4] = rand() / (1.0 * RAND_MAX);
			data[i + 5] = rand() / (1.0 * RAND_MAX);

			data[i + 6] = (data[i] / r + 1.0f) * 0.5f;
			data[i + 7] = (data[i + 1] / r + 1.0f) * 0.5f;

			data[i + 8] = 0.0f;
			data[i + 9] = 0.0f;
			data[i + 10] = -1.0f;

			j++;
		}

		//need 3n indices
		sizeInd = 3 * n;
		points = sizeInd;
		indices = new GLuint[sizeInd];
		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;
		for (int i = 3; i < sizeInd; i += 3) {
			indices[i] = 0;
			indices[i + 1] = indices[i - 1];
			indices[i + 2] = indices[i - 1] + 1;
		}
		indices[sizeInd - 1] = indices[1];

		layout();
	}

	void draw() {
		drawShapeElements(primitive);
	}

	~Circle() {
		delete[] indices;
		delete[] data;
	}

};

class Cylinder : public Shape {
public:

	Cylinder(int n) {

		primitive = GL_TRIANGLES;

		//each vert = 3pos + 3col + 2tex + 3 norm = 11
		//n steps requires n+1 verts including middle
		size = attribCount * (n + 1) * 2;
		data = new GLfloat[size];

		float r = 0.8f;
		float h = 1.0f;
		float pi = 3.14159f;

		data[0] = 0.0f;
		data[1] = -h;
		data[2] = 0.0f;

		data[3] = rand() / (1.0 * RAND_MAX);
		data[4] = rand() / (1.0 * RAND_MAX);
		data[5] = rand() / (1.0 * RAND_MAX);

		data[6] = 0.5f;
		data[7] = 0.5f;

		data[8] = 0.0f;
		data[9] = 0.0f;
		data[10] = -1.0f;

		//bottom circle
		int j = 0;
		int i = attribCount;
		for (; i < (size- attribCount)/2; i += attribCount) {
			data[i] = r * cos(j * 2 * pi / n);
			data[i + 1] = -h;
			data[i + 2] = r * sin(j * 2 * pi / n);	

			data[i + 3] = rand() / (1.0 * RAND_MAX);
			data[i + 4] = rand() / (1.0 * RAND_MAX);
			data[i + 5] = rand() / (1.0 * RAND_MAX);
			

			data[i + 6] = (data[i] / r + 1.0f) * 0.5f;
			data[i + 7] = (data[i + 1] / r + 1.0f) * 0.5f;

			data[i + 8] = 0.0f;
			data[i + 9] = 0.0f;
			data[i + 10] = -1.0f;

			j++;
		}

		//top circle
		j = 0;
		for (; i < size-attribCount; i += attribCount) {
			data[i] = r * cos(j * 2 * pi / n);
			data[i + 1] = h;
			data[i + 2] = r * sin(j * 2 * pi / n);

			data[i + 3] = rand() / (1.0 * RAND_MAX);
			data[i + 4] = rand() / (1.0 * RAND_MAX);
			data[i + 5] = rand() / (1.0 * RAND_MAX);
			
			data[i + 6] = (data[i] / r + 1.0f) * 0.5f;
			data[i + 7] = (data[i + 1] / r + 1.0f) * 0.5f;

			data[i + 8] = 0.0f;
			data[i + 9] = 0.0f;
			data[i + 10] = -1.0f;

			j++;
		}

		data[i] = 0.0f;
		data[i + 1] = h;
		data[i + 2] = 0.0f;

		data[i + 3] = 0.0f;
		data[i + 4] = 0.0f;
		data[i + 5] = 0.0f;

		data[i + 6] = 0.5f;
		data[i + 7] = 0.5f;

		data[i + 8] = 0.0f;
		data[i + 9] = 0.0f;
		data[i + 10] = -1.0f;

		//need 3n indices
		sizeInd = 3 * n + 6 * n + 3 * n;
		points = sizeInd;
		indices = new GLuint[sizeInd];
		
		//bottom
		 
		//indices[0] = 0;
		//indices[1] = 1;
		//indices[2] = 2;

		//indices[3] = 0;				//		n=6 => points = 18
		//indices[4] = 2;				//		
		//indices[5] = 3;				//		 3______2
										//	   	 /		\
		//indices[6] = 0;				//	   4/	0	 \ 1
		//indices[7] = 3;				//		\		 /
		//indices[8] = 4;				//		5\______/6
										//
		//indices[9] = 0;
		//indices[10] = 4;
		//indices[11] = 5;

		//indices[12] = 0;
		//indices[13] = 5;
		//indices[14] = 6;

		//indices[15] = 0;
		//indices[16] = 6;
		//indices[17 == 3n-1] = 1;
		
		i = 0;
		int l = 1;
		for (; i < 3*(n-1); i += 3) {
			indices[i] = 0;				//indices[0] to indices[3n-6]
			indices[i + 1] = l;			//indices[1] to indices[3n-5]
			indices[i + 2] = l + 1;		//indices[2] to indices[3n-4]
			l++;
		}
		indices[i] = 0;			//indices[3n-3]
		indices[i + 1] = l;		//indices[3n-2]
		indices[i + 2] = 1;		//indices[3n-1]

		//hollow cone side

		//indices[18 == 3n] = 7;	//indices[21] = 2;	//		n=6 => points = 18 + 36
		//indices[19] = 1;			//indices[22] = 8;	//
		//indices[20] = 2;			//indices[23] = 7;	//		 9______8
														//	   	 /		\
		//indices[24] = 8;	//indices[27] = 3;			//	  10/	13	 \7			
		//indices[25] = 2;	//indices[28] = 9;			//		\		 /			
		//indices[26] = 3;	//indices[29] = 8;			//	   11\______/12	
														//	
		//indices[30] = 9;	//indices[33] = 4;			
		//indices[31] = 3;	//indices[34] = 10;			
		//indices[32] = 4;	//indices[35] = 9;			
										
		//indices[36] = 10;	//indices[39] = 5;			//		 3______2
		//indices[37] = 4;	//indices[40] = 11;			//	   	 /		\
		//indices[38] = 5;	//indices[41] = 10;			//	   4/	0	 \1
														//		\		 /	
		//indices[42] = 11;	//indices[45] = 6;			//		5\______/6	
		//indices[43] = 5;	//indices[46] = 12;			//
		//indices[44] = 6;	//indices[47] = 11;

		//indices[48] = 12;	//indices[51] = 1;
		//indices[49] = 6;	//indices[52] = 7;
		//indices[50] = 1;	//indices[53 == 18 + 2(18)-1 == 3n + 2(3n)-1] = 12;

		i = 3 * n;
		l = 1;
		for (; i < (3 * n + 2 * 3 * n - 6); i += 6) {
			indices[i] = n + l;				//indices[3n] to indices[3n + 2(3n)-6]
			indices[i + 1] = l;				//indices[3n+1] to indices[3n + 2(3n)-5]
			indices[i + 2] = l + 1;			//indices[3n+2] to indices[3n + 2(3n)-4]

			indices[i + 3] = l + 1;			//indices[3n+3] to indices[3n + 2(3n)-3]
			indices[i + 4] = n + l + 1;		//indices[3n+4] to indices[3n + 2(3n)-2]
			indices[i + 5] = n + l;			//indices[3n+5] to indices[3n + 2(3n)-1]
			l++;
		}
		//i += 6;
		indices[i] = 2*n-(n-1);			//redo last triangle
		indices[i + 1] = n;		//
		indices[i + 2] = 1;			//

		indices[i + 3] = 2*n;			//redo last triangle
		indices[i + 4] = 2 * n - (n - 1);		//
		indices[i + 5] = n;


		//top

		i = 0;								// n = 6 = > points = 18 + 36 + 18 = 72
		int k = 3 * n + 2 * 3 * n;
		l = 1;
		for (; i < 3 * (n - 1); i += 3) {
			indices[k + i] = (n + 1) * 2 - 1;	//indices[3n + 2(3)n] to indices[3n-5] = last vertex
			indices[k + i + 1] = n + l;				//indices[3n + 2(3)n + 1] to indices[3n-5]
			indices[k + i + 2] = n + l + 1;			//indices[3n + 2(3)n + 2] to indices[3n-4]
			l++;
		}
		indices[k + i] = (n + 1) * 2 - 1;		//last vertex
		indices[k + i + 1] = n+l;					//indices[3n-2]
		indices[k + i + 2] = n+1;					//indices[3n-1]

		layout();
	}

	void draw() {
		drawShapeElements(primitive);
	}

	~Cylinder() {
		delete[] indices;
		delete[] data;
	}

};

class Polynomial : Shape {
public:
	// ax^4 + bx^3 + cx^2 + dx + e + r/x + s/x^2	// low < x < high	// n points		// given in y^2
	Polynomial(float a, float b, float c, float d, float e, float r, float s, float low, float high, int n, bool ySquared) {
		float dx = (high - low) / float(n-1);
		points = n;
		size = attribCount *n;
		data = new GLfloat[size];

		float err = 0.00001;
		
		for (int i = 0; i < size; i+= attribCount) {
			
			float x = low + (i / float(attribCount)) * dx;
			
			float A = a * x * x * x * x;
			float B = b * x * x * x;
			float C = c * x * x;
			float D = d * x;
			float E = e;
			float R;
			float S;
			if (abs(x) > err) {
				R = r / x;
				S = s / (x * x);
			}
			else {
				R = std::numeric_limits<float>::max();
				S = std::numeric_limits<float>::max();
			}
			
			float y = A + B + C + D + E + R + S;
			if (ySquared) {
				y = sqrt(y);
			}
			//std::cout << x << " " << y << std::endl;
			x *= 0.15f;
			y *= 0.15f;
			

			data[i] = x;
			data[i + 1] = y;
			data[i + 2] = 0.0f;

			data[i + 3] = 1.0f;
			data[i + 4] = 0.0f;
			data[i + 5] = 0.0f;

			data[i + 6] = 0.0f;
			data[i + 7] = 1.0f;

			data[i + 8] = 0.0f;
			data[i + 9] = 0.0f;
			data[i + 10] = 0.0f;
			
		}

		sizeInd = 2*n;
		points = sizeInd-1;
		primitive = GL_LINES;
		indices = new GLuint[sizeInd];
		int k = 0;
		for (int i = 0; i < sizeInd-1; i+=2) {
			indices[i] = k;
			indices[i + 1] = k + 1;
			k++;
		}


		layout();
	
	}

	void draw() {
		drawShapeElements(primitive);
	}

	~Polynomial() {
		delete[] indices;
		delete[] data;
	}

};

class Cone : public Shape {
public:

	Cone(int n) {

		//each vert = 3pos + 3col + 2tex + 3 norm = 11
		//n steps requires n+2 verts including both middles
		size = attribCount * (n + 2);
		data = new GLfloat[size];

		float h = 1.0f;
		float r = 1.0f;
		float pi = 3.14159f;

		data[0] = 0.0f;
		data[1] = h;
		data[2] = 0.0f;

		data[3] = 0.0f;
		data[4] = 1.0f;
		data[5] = 0.0f;

		data[6] = 0.5f;
		data[7] = 0.5f;

		data[8] = 0.0f;
		data[9] = 0.0f;
		data[10] = -1.0f;

		int j = 0;
		for (int i = attribCount; i < size; i += attribCount) {
			data[i] = r * cos(j * 2 * pi / n);
			data[i + 1] = -h;
			data[i + 2] = r * sin(j * 2 * pi / n);;

			data[i + 3] = rand() / (1.0 * RAND_MAX);
			data[i + 4] = rand() / (1.0 * RAND_MAX);
			data[i + 5] = rand() / (1.0 * RAND_MAX);

			data[i + 6] = (data[i] / r + 1.0f) * 0.5f;
			data[i + 7] = (data[i + 1] / r + 1.0f) * 0.5f;

			data[i + 8] = 0.0f;
			data[i + 9] = 0.0f;
			data[i + 10] = -1.0f;

			j++;
		}

		//need 3n indices
		sizeInd = 3 * n;
		points = sizeInd;
		indices = new GLuint[sizeInd];
		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;
		for (int i = 3; i < sizeInd; i += 3) {
			indices[i] = 0;
			indices[i + 1] = indices[i - 1];
			indices[i + 2] = indices[i - 1] + 1;
		}
		indices[sizeInd - 1] = indices[1];

		layout();
	}

	void draw() {
		drawShapeElements(primitive);
	}

	~Cone() {
		delete[] indices;
		delete[] data;
	}

};

class Sphere:public Shape {
public:

	Sphere(int n) {
		//each vert = 3pos + 3col + 2tex + 3 norm = 11
		//n verts for the circle (no middle)
		//times m+1 for the sphere
		int m = n;
		size = attribCount * (n * (m + 1));
		data = new GLfloat[size];

		float r = 1.0f;
		float pi = 3.14159f;

		int j = 0;
		for (int k = 0; k <= m; k++) {
			for (int i = 0; i < attribCount * n; i += attribCount) {
				data[k * (attribCount * n) + i] = r * cos(j * 2.0f * pi / n) * sin(k * pi / m);
				data[k * (attribCount * n) + i + 1] = r * sin(j * 2.0f * pi / n) * sin(k * pi / m);
				data[k * (attribCount * n) + i + 2] = r * cos(k * pi / m);

				data[k * (attribCount * n) + i + 3] = rand() / (1.0 * RAND_MAX);
				data[k * (attribCount * n) + i + 4] = rand() / (1.0 * RAND_MAX);
				data[k * (attribCount * n) + i + 5] = rand() / (1.0 * RAND_MAX);
			
				data[k * (attribCount * n) + i + 6] = (data[k * (attribCount * n) + i] / r + 1) * 0.5f;
				data[k * (attribCount * n) + i + 7] = (data[k * (attribCount * n) + i + 1] / r + 1) * 0.5f;

				data[k * (attribCount * n) + i + 8] = 0.0f;
				data[k * (attribCount * n) + i + 9] = 0.0f;
				data[k * (attribCount * n) + i + 10] = -1.0f;

				j++;
			}
		}

		sizeInd = 6 * (n+1) * m;
		primitive = GL_TRIANGLES;
		points = sizeInd;
		indices = new GLuint[sizeInd];
		int k = 0;
		for (int j = 0; j < m; j++) {
			for (int i = 0; i < n-1; i++) {
				
				indices[k] = i + (j + 1) * n;;
				indices[k+1] = i + j * n;
				indices[k+2] = i + 1 + (j + 1) * n;

				indices[k+3] = i + j * n;
				indices[k+4] = i + 1 + j * n;
				indices[k+5] = i + 1 + (j + 1) * n;

				k += 6;
			}

			indices[k] = n-1 + (j + 1) * n;;
			indices[k + 1] = n-1 + j * n;
			indices[k + 2] = (j+1)*n;

			indices[k + 3] = n-1 + j * n;
			indices[k + 4] = j * n;
			indices[k + 5] = (j + 1)* n;

			k += 6;
		}
		//indices[sizeInd - 1] = indices[1];

		layout();
	}

	void draw() {
		drawShapeElements(primitive);
	}

	~Sphere() {
		delete[] indices;
		delete[] data;
	}
};


struct Torus : public Shape {

	Torus(int n) {
		int m = n;
		points = n * m;

		float pi = 3.14159f;

		float R = 0.5f;
		float r = R / 2;

		size = n * m * attribCount;
		data = new GLfloat[size];


		int j = 0;
		for (int k = 0; k < m; k++) {
			for (int i = 0; i < attribCount * n; i += attribCount) {

				int offset = k * attribCount * n;
				GLfloat A = (r * cos(i * 2.0f * pi / (attribCount * n)) + R) * cos(j * 2.0f * pi / (m));
				GLfloat B = r * sin(i * 2.0f * pi / (attribCount * n));
				GLfloat C = (r * cos(i * 2.0f * pi / (attribCount * n)) + R) * sin(j * 2.0f * pi / (m));

				data[offset + i] = C;
				data[offset + i + 1] = A;
				data[offset + i + 2] = B;

				data[offset + i + 3] = rand() / (1.0 * RAND_MAX);
				data[offset + i + 4] = rand() / (1.0 * RAND_MAX);
				data[offset + i + 5] = rand() / (1.0 * RAND_MAX);

				data[offset + i + 6] = 0.0;
				data[offset + i + 7] = 0.0;

				data[offset + i + 8] = 0.0;
				data[offset + i + 9] = 0.0;
				data[offset + i + 10] = 0.0;
			}
			j++;
		}

		sizeInd = 6 * (n) * (m);
		primitive = GL_TRIANGLES;
		points = sizeInd;
		indices = new GLuint[sizeInd];
		int k = 0;
		for (int j = 0; j < m-1; j++) {
			for (int i = 0; i < n - 1; i++) {

				indices[k] = i + (j + 1) * n;
				indices[k + 1] = i + j * n;
				indices[k + 2] = i + 1 + (j + 1) * n;

				indices[k + 3] = i + j * n;
				indices[k + 4] = i + 1 + j * n;
				indices[k + 5] = i + 1 + (j + 1) * n;

				k += 6;
			}

			indices[k] = n - 1 + (j + 1) * n;
			indices[k + 1] = n - 1 + j * n;
			indices[k + 2] = (j + 1) * n;

			indices[k + 3] = n - 1 + j * n;
			indices[k + 4] = j * n;
			indices[k + 5] = (j + 1) * n;

			k += 6;
		}
	
	
		for (int i = 0; i < n - 1; i++) {

			indices[k] = m * (n - 1) + i;
			indices[k + 1] = m * (n - 1) + i + 1;
			indices[k + 2] = i;

			indices[k + 3] = m * (n - 1) + i + 1;
			indices[k + 4] = i + 1;
			indices[k + 5] = i;

			k += 6;
		}
		
		indices[k ] = m*(n-1);
		indices[k+1] = n*m - 1;
		indices[k + 2] = 0;

		indices[k + 3] = n * m - 1;
		indices[k + 4] = n-1;
		indices[k + 5] = 0;

		layout();
	}

	void draw() {
		drawShapeElements(primitive);
	}

	~Torus() {
		delete[] indices;
		delete[] data;
	}

};

struct StarTorus : public Shape {

	StarTorus(int n) {
		int m = n;
		points = n * m;

		float pi = 3.14159f;

		float R = 0.5;
		float r = R / 2;

		size = n * m * attribCount;
		data = new GLfloat[size];


		int j = 0;
		for (int k = 0; k < m; k++) {
			for (int i = 0; i < attribCount * n; i += attribCount) {

				int offset = k * attribCount * n;
		
				GLfloat A = (r * cos(i * 2.0f * pi / n) + R) * cos(j * 2.0f * pi / m);
				GLfloat B = r * sin(i * 2.0f * pi / n);
				GLfloat C = (r * cos(i * 2.0f * pi / n) + R) * sin(j * 2.0f * pi / m);

				data[offset + i] = C;
				data[offset + i + 1] = A;
				data[offset + i + 2] = B;

				data[offset + i + 3] = rand() / (1.0 * RAND_MAX);
				data[offset + i + 4] = rand() / (1.0 * RAND_MAX);
				data[offset + i + 5] = rand() / (1.0 * RAND_MAX);

				data[offset + i + 6] = 0.0;
				data[offset + i + 7] = 0.0;

				data[offset + i + 8] = 0.0;
				data[offset + i + 9] = 0.0;
				data[offset + i + 10] = 0.0;
			}
			j++;
		}

		sizeInd = 6 * (n + 1) * m;
		primitive = GL_TRIANGLES;
		points = sizeInd - m * 6 * 4;
		indices = new GLuint[sizeInd];
		int k = 0;
		for (int j = 0; j < m; j++) {
			for (int i = 0; i < n - 1; i++) {

				indices[k] = i + (j + 1) * n;;
				indices[k + 1] = i + j * n;
				indices[k + 2] = i + 1 + (j + 1) * n;

				indices[k + 3] = i + j * n;
				indices[k + 4] = i + 1 + j * n;
				indices[k + 5] = i + 1 + (j + 1) * n;

				k += 6;
			}

			indices[k] = n - 1 + (j + 1) * n;;
			indices[k + 1] = n - 1 + j * n;
			indices[k + 2] = (j + 1) * n;

			indices[k + 3] = n - 1 + j * n;
			indices[k + 4] = j * n;
			indices[k + 5] = (j + 1) * n;

			k += 6;
		}
		indices[sizeInd - 1] = indices[1];

		layout();
	}

	void draw() {
		drawShapeElements(primitive);
	}

	~StarTorus() {
		delete[] indices;
		delete[] data;
	}

};

#endif
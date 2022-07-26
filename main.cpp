#include <GL/glut.h>
#include <time.h>
#include <iostream>
#include <iomanip>
#include <math.h> 
#include <queue>

GLint winWidth = 1000, winHeight = 1000; // Tamaño inicial ventana de visualización.
GLfloat xref = 50.0, yref = 50.0, zref = 0.0; // Punto observado.
GLfloat Vx = 0.0, Vy = 1.0, Vz = 0.0; // Vector vertical.

double range = 4; // +-10
int n = 4;
int size = pow(2, n) + 1;

class DiamondSquare
{
	private:
		double random_range;
		double min_val;
		double max_val;
		
		double** map;
		int size;
		
		int range;
		
	public:
		
		DiamondSquare(double** array, int s)
		{
			map = array;
			size = s;
			range = 2;
		}
		
		~DiamondSquare()
		{
			//@TODO delete map
			for (int i = 0; i < size; i++)
			{
				delete map[i];
			}
			delete map;
		}
		
		double** process()
		{
			_on_start();
			
			//Processing...
			for (int sideLength = size - 1; sideLength >= 2; sideLength /= 2, range /= 2)
			{
				int halfSide = sideLength / 2;
				
				squareStep(sideLength, halfSide);
				diamondStep(sideLength, halfSide);
			}
			_on_end();
			
			return map;
		}
		
		
		void _on_start()
		{
			// Defining the corners values :
			map[0][0] = map[0][size - 1] = map[size - 1][0] = map[size - 1][size - 1] = 10;
			// Initializing srand for random values :
			srand(time(NULL));
		}
		
		void _on_end()
		{
			
		}
		
		void diamondStep(int sideLength, int halfSide)
		{
			for (int x = 0; x < size - 1; x += halfSide)
			{
				for (int y = (x + halfSide) % sideLength; y < size - 1; y += sideLength)
				{
					double avg = map[(x - halfSide + size - 1) % (size - 1)][y] +
						map[(x + halfSide) % (size - 1)][y] +
						map[x][(y + halfSide) % (size - 1)] +
							map[x][(y - halfSide + size - 1) % (size - 1)];
							avg /= 4.0 + dRand(-range, range);
							map[x][y] = avg;
							
							if (x == 0) map[size - 1][y] = avg;
							if (y == 0) map[x][size - 1] = avg;
				}
			}
		}
		
		void squareStep(int sideLength, int halfSide)
		{
			for (int x = 0; x < size - 1; x += sideLength)
			{
				for (int y = 0; y < size - 1; y += sideLength)
				{
					double avg = map[x][y] + map[x + sideLength][y] + map[x][y + sideLength] + map[x + sideLength][y + sideLength];
					avg /= 4.0;
					map[x + halfSide][y + halfSide] = avg + dRand(-range, range);
				}
			}
		}
		
		double dRand(double dMin, double dMax)
		{
			double d = (double)rand() / RAND_MAX;
			return dMin + d * (dMax - dMin);
		}
};

void displayTable(double** map, int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			//std::cout << map[i][j] << " " ;
			std::cout << std::setprecision(2) << std::fixed << map[i][j] << " ";
		}
		std::cout << "" << std::endl;
	}
}

double** gen_table(int factor)
{
	int size = factor;
	
	double** map = new double* [size];
	for (int i = 0; i < size; i++)
	{
		map[i] = new double[size];
		for (int j = 0; j < size; j++)
			map[i][j] = 0.0;
	}
	return map;
}

void init (void)
{
	glClearColor (0.121, 0.121, 0.121, 0.0);
	glMatrixMode (GL_MODELVIEW);
	gluLookAt (-5.0, 20.0, -5.0, size, 0, size, 0, 1, 0);
	glMatrixMode (GL_PROJECTION);
	glFrustum (-6, 6, -8, 8, 11, 3200);
}

void displayFcn (void)
{
	glClear (GL_COLOR_BUFFER_BIT);
	
	std::cout << "Size : " << size << std::endl;
	
	double** map1 = gen_table(size);
	
	DiamondSquare ds(map1, size);
	double** map = ds.process();
	
	std::queue<float> heights;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			heights.push((float) map[i][j]);
		}
	}
	
	int num = (int) size * size;
	
	double** x = new double* [size];
	for (int i = 0; i < size; i++)
	{
		x[i] = new double[size];
		for (int j = 0; j < size; j++)
			x[i][j] = i;
	}
	
	std::queue<float> xCoordinates;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			xCoordinates.push((float)x[i][j]);
		}
	}
	
	double** z = new double* [size];
	for (int i = 0; i < size; i++)
	{
		z[i] = new double[size];
		for (int j = 0; j < size; j++)
			z[i][j] = j;
	}
	
	std::queue<float> zCoordinates;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			zCoordinates.push((float)z[i][j]);
		}
	}
	
	float ***data = new float** [size];
	for(int i = 0; i < size; i++)
	{
		data[i] = new float*[size];
		for(int j = 0; j < size; j++)
		{
			data[i][j] = new float[3];
		}
	}
	
	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < size; j++)
		{
			float x = xCoordinates.front();
			float y = heights.front();
			float z = zCoordinates.front();
			
			
			
			data[i][j][0] = x;
			data[i][j][1] = y;
			data[i][j][2] = z;
			
			xCoordinates.pop();
			zCoordinates.pop();
			heights.pop();
		}
	}
	
	int k=0;
	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < size; j++)
		{
			if((i+1 < size) && (j+1 < size))
			{
				float x1 = data[i][j][0];
				float y1 = data[i][j][1];
				float z1 = data[i][j][2];
				
				float x2 = data[i+1][j][0];
				float y2 = data[i+1][j][1];
				float z2 = data[i+1][j][2];
				
				float x3 = data[i][j+1][0];
				float y3 = data[i][j+1][1];
				float z3 = data[i][j+1][2];
				
				float x4 = data[i+1][j+1][0];
				float y4 = data[i+1][j+1][1];
				float z4 = data[i+1][j+1][2];
				
				glColor3f (0.9, 0.9, 0.9); // Seleccionar color de relleno verde.
				glPolygonMode (GL_FRONT, GL_LINE);
				glPolygonMode (GL_BACK, GL_LINE); // Cara posterior alámbrica.
				glBegin (GL_TRIANGLES);
				
				glVertex3f (x1, y1, z1);
				glVertex3f (x2, y2, z2);
				glVertex3f (x3, y3, z3);
				
				glEnd ( );
				glFlush ( );
				glColor3f (0.9, 0.9, 0.9); // Seleccionar color de relleno verde.
				glPolygonMode (GL_FRONT, GL_LINE);
				glPolygonMode (GL_BACK, GL_LINE); // Cara posterior alámbrica.
				glBegin (GL_TRIANGLES);
				
				glVertex3f (x2, y2, z2);
				glVertex3f (x3, y3, z3);
				glVertex3f (x4, y4, z4);
				
				glEnd ( );
				glFlush ( );
				k++;
			}
		}
	}
	glFlush ( );
}

void reshapeFcn (GLint newWidth, GLint newHeight)
{
	glViewport (0, 0, newWidth, newHeight);
	winWidth = newWidth;
	winHeight = newHeight;
}

int main (int argc, char** argv)
{
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition (50, 50);
	glutInitWindowSize (winWidth, winHeight);
	glutCreateWindow ("Terrain Generation");
	init ( );
	glutDisplayFunc (displayFcn);
	glutReshapeFunc (reshapeFcn);
	glutMainLoop ( );
	return 0;
}

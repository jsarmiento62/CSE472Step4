#include "StdAfx.h"
#include "Sphere.h"
#include <cmath>

const double GR_PI = 3.1415926535897932384626433832795;

inline void Normalize3(GLdouble *v)
{
   GLdouble len = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
   v[0] /= len;
   v[1] /= len;
   v[2] /= len;
}


CSphere::CSphere(void)
{
    m_radius = 5;
    m_texture = NULL;
}

CSphere::~CSphere(void)
{
}


void CSphere::InitGL()
{
   GLdouble a[] = {1, 0, 0};
   GLdouble b[] = {0, 0, -1};
   GLdouble c[] = {-1, 0, 0};
   GLdouble d[] = {0, 0, 1};
   GLdouble e[] = {0, 1, 0};
   GLdouble f[] = {0, -1, 0};

   int recurse = 7;

   SphereFace(recurse, m_radius, d, a, e);
   SphereFace(recurse, m_radius, a, b, e);
   SphereFace(recurse, m_radius, b, c, e);
   SphereFace(recurse, m_radius, c, d, e);
   SphereFace(recurse, m_radius, a, d, f);
   SphereFace(recurse, m_radius, b, a, f);
   SphereFace(recurse, m_radius, c, b, f);
   SphereFace(recurse, m_radius, d, c, f);


   glGenVertexArrays(1, &m_vao);
   glBindVertexArray(m_vao);

   glGenBuffers(1, &m_vertexVBO);
   glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO);
   glBufferData(GL_ARRAY_BUFFER, vertexArray.size()*sizeof(glm::vec3),
	   &vertexArray[0], GL_STATIC_DRAW);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

   glGenBuffers(1, &m_normalVBO);
   glBindBuffer(GL_ARRAY_BUFFER, m_normalVBO);
   glBufferData(GL_ARRAY_BUFFER, normalArray.size()*sizeof(glm::vec3),
	   &normalArray[0], GL_STATIC_DRAW);
   glEnableVertexAttribArray(1);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

   glGenBuffers(1, &m_texVBO);
   glBindBuffer(GL_ARRAY_BUFFER, m_texVBO);
   glBufferData(GL_ARRAY_BUFFER, texArray.size()*sizeof(glm::vec2),
	   &texArray[0], GL_STATIC_DRAW);
   glEnableVertexAttribArray(2);
   glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
}



void CSphere::SphereFace(int p_recurse, double p_radius, double *a,
                            double *b, double *c)
{
    if(p_recurse > 1)
    {
       // Compute vectors halfway between the passed vectors 
       GLdouble d[3] = {a[0] + b[0], a[1] + b[1], a[2] + b[2]};
       GLdouble e[3] = {b[0] + c[0], b[1] + c[1], b[2] + c[2]};
       GLdouble f[3] = {c[0] + a[0], c[1] + a[1], c[2] + a[2]};

       Normalize3(d);
       Normalize3(e);
       Normalize3(f);

       SphereFace(p_recurse-1, p_radius, a, d, f);
       SphereFace(p_recurse-1, p_radius, d, b, e);
       SphereFace(p_recurse-1, p_radius, f, e, c);
       SphereFace(p_recurse-1, p_radius, f, d, e);
    }
    else 
    {
			// What's the texture coordinate for this normal?
            double tx1 = atan2(a[0], a[2]) / (2. * GR_PI) + 0.5;
            double ty1 = asin(a[1]) / GR_PI + .5;
			texArray.push_back(glm::vec2(tx1, ty1)); 
			normalArray.push_back(glm::vec3(a[0], a[1], a[2]));
			vertexArray.push_back(glm::vec3(a[0] * p_radius, a[1] * p_radius, a[2] * p_radius));

			double tx2 = atan2(b[0], b[2]) / (2. * GR_PI) + 0.5;
            double ty2 = asin(b[1]) / GR_PI + .5;
			// Test for this coordinate on the other side of the
            // texture from the first coordinate.
            if(tx2 < 0.75 && tx1 > 0.75)
                tx2 += 1.0;
            else if(tx2 > 0.75 && tx1 < 0.75)
                tx2 -= 1.0;
			texArray.push_back(glm::vec2(tx2, ty2));
			normalArray.push_back(glm::vec3(b[0],b[1],b[2]));
			vertexArray.push_back(glm::vec3(b[0] * p_radius, b[1] * p_radius, b[2] * p_radius));

			
			double tx3 = atan2(c[0], c[2]) / (2. * GR_PI) + 0.5;
            double ty3 = asin(c[1]) / GR_PI + .5;
			// Test for this coordinate on the other side of the
            // texture from the first coordinate.
            if(tx3 < 0.75 && tx1 > 0.75)
                tx3 += 1.0;
            else if(tx3 > 0.75 && tx1 < 0.75)
                tx3 -= 1.0;
			texArray.push_back(glm::vec2(tx3, ty3));
			normalArray.push_back(glm::vec3(c[0],c[1],c[2]));
			vertexArray.push_back(glm::vec3(c[0] * p_radius, c[1] * p_radius, c[2] * p_radius));
    }
}

void CSphere::RenderGL()
{
	glUseProgram(m_program);
	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, vertexArray.size());
}

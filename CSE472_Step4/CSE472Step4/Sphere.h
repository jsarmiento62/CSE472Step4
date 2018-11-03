#pragma once
#include "ShaderWnd\ShaderWnd.h"
#include "gl/gl.h"
#include "graphics/GrTexture.h"

class CSphere
{
public:
    CSphere(void);
    ~CSphere(void);

    void SetRadius(double r) {m_radius = r;}

private:
    void SphereFace(int p_recurse, double p_radius, double *a,
                            double *b, double *c);

    double m_radius;
	
	CGrTexture   *m_texture;  // Texture to use for the sphere

public:	
	void SetTexture(CGrTexture *texture) {m_texture = texture;}
    CGrTexture *GetTexture() {return m_texture;}


public:
	GLuint m_program;
	GLuint m_vao;
	GLuint m_vertexVBO;
	GLuint m_normalVBO;
	GLuint m_texVBO;

	void InitGL();
	void RenderGL();
	void CleanGL();

private:
	std::vector<glm::vec3> vertexArray;
	std::vector<glm::vec3> normalArray;
	std::vector<glm::vec2> texArray;

};


//Simple shader-based OpenGL windows using GLEW
// Y. Tong 04/18/2013

#pragma once

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

#include <stdlib.h>
#include <string.h>

#define GLEW_STATIC
#include "external/glew-1.9.0/include/GL/glew.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include "external/glew-1.9.0/include/GL/glext.h"
#include "external/glew-1.9.0/include/GL/wglext.h"
#include "external/glm/glm.hpp"
#include "external/glm/gtc/constants.hpp"
#include "external/glm/gtc/matrix_transform.hpp"
#include "external//glm/gtc/type_ptr.hpp"
#include <math.h>


using namespace glm;
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
/////////////////////////////////////////////////////////////////////////////
// CShaderWnd window
class CShaderWnd : public CWnd
{
public:
	CShaderWnd();
	virtual ~CShaderWnd();

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShaderWnd)
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CShaderWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	bool CreateOpenGLContext(CDC* pDC);
	GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);

	HGLRC m_hGLRC; // OpenGL Resource Context
	GLuint m_program; // program object containing shaders
	virtual void RenderGL();
	virtual void InitGL();
	virtual void CleanGL();

	bool m_bManageTransformation;
	mat4 m_mModel;
	mat4 m_mView;
	mat4 m_mProjection;
	mat4 m_mPVM;
	mat4 m_mVM;
	GLuint m_nPVM;
	GLuint m_nVM;
	mat4 m_mRotation;

	vec3 m_vEye;
	vec3 m_vLookat;
	vec3 m_vUp;
	float m_fNear, m_fFar, m_fFoV;

	int m_nXRes;
	int m_nYRes;
	bool m_bLButtonDown;
	bool m_bRButtonDown;
	enum TrackBallMode {TRACKBALL_NONE=0, TRACKBALL_ROTATE=1, TRACKBALL_PAN=2, TRACKBALL_ZOOM=3};
	TrackBallMode m_mode;
	vec3 m_vCenter;
	vec3 m_vDown;
	float m_fRadius;
	bool m_bDragging;
	bool m_bMoving;
	void HandleButtons(int x, int y);
	vec3 Intersect(int x, int y);
	void Motion(vec3& vVec);
	void UpdateMatrix();
	void ResetMatrix();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "winmm.lib")

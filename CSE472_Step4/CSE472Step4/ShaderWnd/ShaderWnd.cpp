// ShaderWnd.cpp : implementation file
//

#include "stdafx.h"


#include "ShaderWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CShaderWnd

CShaderWnd::CShaderWnd()
{
	m_hGLRC = NULL;
	m_program = 0;

	m_bManageTransformation = true;
	m_vEye = vec3(0.f, 0.f, 25.f);
	m_vLookat = vec3(0.f, 0.f, 0.f);
	m_vUp = vec3(0.f, 1.f, 0.f);
	m_fNear = 0.1f;
	m_fFar = 1000.f;
	m_fFoV = 30.f;
	m_mView = mat4(1.f);
	m_mModel = mat4(1.f);
	m_mVM = mat4(1.f);
	m_mRotation = mat4(1.f);

	m_vCenter = vec3(0.f, 0.f, 0.f);
	m_fRadius = 1.f;
	m_bDragging = false;
	m_mode = TRACKBALL_NONE;
	m_bMoving = false;
	m_vDown = vec3(0.f);
	m_bLButtonDown = false;
	m_bRButtonDown = false;
}

CShaderWnd::~CShaderWnd()
{
}


BEGIN_MESSAGE_MAP(CShaderWnd, CWnd)
//{{AFX_MSG_MAP(CShaderWnd)
ON_WM_CREATE()
ON_WM_PAINT()
ON_WM_ERASEBKGND()
ON_WM_DESTROY()
ON_WM_SIZE()
ON_WM_MOUSEMOVE()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_RBUTTONDOWN()
ON_WM_RBUTTONUP()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShaderWnd message handlers

BOOL CShaderWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN ;

	return true;
}

int CShaderWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CClientDC dc(this) ;
	if (!CreateOpenGLContext(&dc))
		return -1;

	BOOL bResult = wglMakeCurrent(dc.m_hDC, m_hGLRC);
	if (!bResult)
	{
		return -1;
	}

	InitGL();
	return 0;
}

void CShaderWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	BOOL bResult = wglMakeCurrent(dc.m_hDC, m_hGLRC);
	if (!bResult)
	{
		return;
	}

	RenderGL();
	SwapBuffers(dc.m_hDC);
}

BOOL CShaderWnd::OnEraseBkgnd(CDC* pDC) 
{
	return true; //glClear does it.
}

bool CShaderWnd::CreateOpenGLContext(CDC* pDC)
{
	HINSTANCE hInst = (HINSTANCE) GetWindowLongPtr(m_hWnd,GWLP_HINSTANCE);

	wchar_t windowClassName[256];
	GetClassName(m_hWnd, windowClassName, 256);

	HWND hWndForGLEW = CreateWindow(windowClassName, _T("FAKE"), WS_OVERLAPPEDWINDOW | WS_MAXIMIZE | WS_CLIPCHILDREN, 0, 0, CW_USEDEFAULT, 
		CW_USEDEFAULT, NULL, NULL, hInst, NULL);

	HDC hDC = ::GetDC(hWndForGLEW);

	PIXELFORMATDESCRIPTOR pfd ;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1; 
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;  
	pfd.iPixelType = PFD_TYPE_RGBA; 
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32; 
	pfd.iLayerType = PFD_MAIN_PLANE;

	int nPixelFormat = ChoosePixelFormat(hDC, &pfd);

	if (nPixelFormat == 0) {
		nPixelFormat = 1;
		if(DescribePixelFormat(hDC,nPixelFormat,
			sizeof(PIXELFORMATDESCRIPTOR),&pfd)==0)
			return FALSE;
	} 

	if (!SetPixelFormat (hDC, nPixelFormat, &pfd) )
		return false; 

	HGLRC GLRC = wglCreateContext(hDC); 

	if (GLRC == NULL)
		return false;

	wglMakeCurrent(hDC, GLRC);

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		AfxMessageBox(_T("GLEW is not supported!"));
	}

	int major, minor;
	char* ver = (char*)glGetString(GL_VERSION); // ver = "3.2.0"

	major = ver[0] - '0';
	if( major >= 3)
	{
		// for GL 3.x
		glGetIntegerv(GL_MAJOR_VERSION, &major); // major = 3
		glGetIntegerv(GL_MINOR_VERSION, &minor); // minor = 2
	}
	else
	{
		minor = ver[2] - '0';
	}

	if( major < 3 || ( major == 3 && minor < 2 ) ) {
		//AfxMessageBox(_T("OpenGL 3.2 is not supported!"));
	}

	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;
	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC) wglGetProcAddress("wglCreateContextAttribsARB");
	PFNWGLCHOOSEPIXELFORMATEXTPROC wglChoosePixelFormatARB = NULL;
	wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATEXTPROC) wglGetProcAddress("wglChoosePixelFormatARB");

	if(wglCreateContextAttribsARB && wglChoosePixelFormatARB)
	{
		const int iPixelFormatAttribList[] = 
		{ 
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE, 
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE, 
			WGL_DOUBLE_BUFFER_ARB, GL_TRUE, 
			WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB, 
			WGL_COLOR_BITS_ARB, 32, 
			WGL_DEPTH_BITS_ARB, 24, 
			WGL_STENCIL_BITS_ARB, 8, 
			0 // End of attributes list
		}; 
		int iContextAttribs[] = 
		{ 
			WGL_CONTEXT_MAJOR_VERSION_ARB, major, 
			WGL_CONTEXT_MINOR_VERSION_ARB, minor, 
			WGL_CONTEXT_FLAGS_ARB, 0,//WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB, 
			0 // End of attributes list
		}; 

		int iPixelFormat, iNumFormats; 
		wglChoosePixelFormatARB(pDC->m_hDC, iPixelFormatAttribList, NULL, 1, &iPixelFormat, (UINT*)&iNumFormats); 

		// PFD seems to be only redundant parameter now
		if(!SetPixelFormat(pDC->m_hDC, iPixelFormat, &pfd))
			return false; 

		HGLRC hRC = wglCreateContextAttribsARB(pDC->m_hDC, 0, iContextAttribs); 
		// If everything went OK
		if(hRC) 
			wglMakeCurrent(pDC->m_hDC, hRC); 
		else
			return false; 

		m_hGLRC = hRC;
	} else {
		m_hGLRC = NULL;
	}

	if (!m_hGLRC) 
	{
		return false; 
	}

	wglMakeCurrent(NULL,NULL);
	wglDeleteContext(GLRC);
	::DestroyWindow(hWndForGLEW);
	return true; 
}

GLuint CShaderWnd::LoadShaders(const char * vertex_file_path,const char * fragment_file_path)
{
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}else{
		TRACE("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::string Line = "";
		while(getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}



	GLint Result = GL_FALSE;
	int InfoLogLength;



	// Compile Vertex Shader
	TRACE("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		TRACE("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	TRACE("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		TRACE("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	TRACE("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		TRACE("%s\n", &ProgramErrorMessage[0]);
	}

	//Mark the shaders for deletion when no longer in use.
	//glDeleteShader(VertexShaderID);
	//glDeleteShader(FragmentShaderID);
	glUseProgram(ProgramID);

	return ProgramID;
}

void CShaderWnd::RenderGL()
{
	// Clear the screen
	glClear( GL_COLOR_BUFFER_BIT );
}

void CShaderWnd::InitGL()
{
	glClearColor(1.0,1.0,1.,1.);
}

void CShaderWnd::CleanGL()
{

}

void CShaderWnd::OnDestroy()
{
	CPaintDC dc(this); // device context for painting

	BOOL bResult = wglMakeCurrent(dc.m_hDC, m_hGLRC);
	if (!bResult)
	{
		return;
	}
	CleanGL();

	if(wglGetCurrentContext() != NULL)
		wglMakeCurrent(NULL,NULL);

	if(m_hGLRC != NULL)
	{
		wglDeleteContext(m_hGLRC);
		m_hGLRC = NULL;
	}
}

void CShaderWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	HWND hWnd = GetSafeHwnd();
	HDC hDC = ::GetDC(hWnd);
	wglMakeCurrent(hDC,m_hGLRC);

	glViewport(0,0, cx,cy);
	m_nXRes = cx;
	m_nYRes = cy;
	UpdateMatrix();
}

void CShaderWnd::ResetMatrix()
{
	m_vCenter = vec3(0.f);
	m_mModel = mat4(1.f);
	m_mRotation = mat4(1.f);

	UpdateMatrix();
}


void CShaderWnd::UpdateMatrix()
{
	mat4 mTranslate = translate(mat4(1.f), m_vCenter);
	m_mProjection = perspective<float>(m_fFoV, m_nXRes/(float)m_nYRes, m_fNear, m_fFar);
	m_mView = lookAt(m_vEye, m_vLookat, m_vUp);

	m_mPVM = m_mProjection * m_mView * mTranslate * m_mRotation * m_mModel;
	m_mVM = m_mView * mTranslate * m_mRotation * m_mModel;
	InvalidateRect(NULL,FALSE);
}

vec3 CShaderWnd::Intersect(int x, int y)
{
	int xc = m_nXRes/2;
	int yc = m_nYRes/2;

	y = m_nYRes -y;
	float r = (float) m_nXRes -xc;
	r = (m_nYRes-yc) < r? (m_nYRes -yc): r;
	r = r;///sqrt(2.f);
	vec3 vOut, p;
	float len;
	float screenWidth;
	switch (m_mode) {
	case TRACKBALL_NONE:
		break;
	case TRACKBALL_ROTATE:
		vOut = vec3( (x-xc)/r, (y-yc)/r, 0.f);
		len = dot(vOut, vOut);
		if (len <= 0.5)
			vOut.z = sqrt(1-len);
		else
			vOut.z = 0.5f/sqrt(len);
		vOut/=length(vOut);
		break;
	case TRACKBALL_PAN:
		vOut = vec3( (float) x/m_nXRes, (float) y/m_nYRes, 0.f);
		screenWidth = ((float) m_nXRes)/m_nYRes * m_fNear * 2.f * tan ((m_fFoV/180*acos(-1.f))/2.f);
		vOut *= dot(m_vCenter-m_vEye, m_vLookat-m_vEye) *screenWidth;
		break;
	case TRACKBALL_ZOOM:
		vOut= vec3(0.f, (float) y/m_nYRes, 0.f);
		vOut *= .01f * (m_fFar-m_fNear);
		break;
	default:
		break;
	}

	return vOut;
}

void CShaderWnd::HandleButtons (int x, int y)
{
// handle mouse button status
	vec3 vVec = Intersect(x,y);
	//End dragging
	if (m_bDragging) {
		Motion(vVec);
		if (m_mode == TRACKBALL_ROTATE)
		{
			m_mModel = m_mRotation * m_mModel;
			m_mRotation = mat4(1.f);
		}
		m_vDown = vec3(0.f);
		m_bDragging = false;
	}
	m_mode = (TrackBallMode) (m_bLButtonDown+2*m_bRButtonDown);
	vVec = Intersect(x,y);
	//Begin dragging
	if (! (m_mode == TRACKBALL_NONE))
	{
		m_vDown = vVec;
		m_bDragging = true;
	}
}

void CShaderWnd::Motion(vec3& vVec)
{
	if (!m_bDragging)
		return;

	vec3 axis;
	float sine;
	float angle;
	switch(m_mode)
	{
	case TRACKBALL_ROTATE:
		axis = cross(m_vDown, vVec);
		sine = length(axis);
		angle = 180.f/acos(-1.f)* atan2(sine,dot(m_vDown,vVec));
		if (sine > 1.e-10f)
			axis /= sine;
		else 
			axis = vec3(1,0,0);
		m_mRotation = rotate(mat4(1.f), angle, axis);
		break;
	case TRACKBALL_PAN:
		m_vCenter += vVec - m_vDown;
		m_vDown = vVec;
		break;
	case TRACKBALL_ZOOM:
		m_vCenter.z = m_vCenter.z + m_vDown[1]-vVec[1];
		m_vDown = vVec;
		break;
	}
}

// mouse move
void CShaderWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_bLButtonDown || m_bRButtonDown)
	{
		vec3 vVec = Intersect(point.x,point.y);
		Motion(vVec);
		m_bMoving = true;
		UpdateMatrix();
	}
	else
		m_bMoving = false;
}

void CShaderWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();
	m_bLButtonDown = TRUE;

	HandleButtons(point.x,point.y);
	CWnd::OnLButtonDown(nFlags, point);
	UpdateMatrix();
}

void CShaderWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	m_bLButtonDown = FALSE;
	m_bMoving = false;
	HandleButtons(point.x,point.y);
	CWnd::OnLButtonUp(nFlags, point);
	UpdateMatrix();
}

void CShaderWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();
	m_bRButtonDown = TRUE;
	HandleButtons(point.x,point.y);
	CWnd::OnRButtonDown(nFlags, point);
}

void CShaderWnd::OnRButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	m_bRButtonDown = FALSE;
	m_bMoving = false;
	HandleButtons(point.x,point.y);
	CWnd::OnRButtonUp(nFlags, point);
	UpdateMatrix();
}

#include "stdafx.h"
#include "World.h"
#include "TorsionDLO.h"



void World::Init(int argc, char** argv)
{
	cout << "Initializing display.. " << endl;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(500,500);
	glutCreateWindow("Inphlexion DLO integration playgoround");
	glEnable(GL_DEPTH_TEST);

	//! initialize shader stuff
	glewInit();

	if (glewIsSupported("GL_VERSION_2_0"))
		printf("Ready for OpenGL 2.0\n");
	else {
		printf("OpenGL 2.0 not supported\n");
		exit(1);
	}

	mainShaderProgram.reset(new FXZProgram());
	shared_ptr<FXZShader> vertexShader(new FXZShader(GL_VERTEX_SHADER));
	vertexShader->setSource("vshader.vert");
	vertexShader->compileShader();
	vertexShader->infoLog();
	shared_ptr<FXZShader> fragmentShader(new FXZShader(GL_FRAGMENT_SHADER));
	fragmentShader->setSource("fshader.frag");
	fragmentShader->compileShader();
	fragmentShader->infoLog();

	mainShaderProgram->attachShader(vertexShader.get());
	mainShaderProgram->attachShader(fragmentShader.get());
	mainShaderProgram->linkProgram();
	mainShaderProgram->infoLog();
	mainShaderProgram->useProgram();

	mainShaderProgram->addUniform("lightDir", UNIFORM3);
	mainShaderProgram->setUniform("lightDir", 1, 0 , 0);
	
	vector<vec3<Real> > positions;
	int n = 50;
	for (int i = 0; i < n; ++i)
	{
		positions.push_back(vec3<Real>(5 * cos(i/4.),5 * sin(i/4.),i/4.));
	}
	IPhysicalObject* dlo = new TorsionDLO(positions, .3, 0.1);
	dlo->SetDampingCoefficient(0.01);
	dlo->SetKl(100);
	dlo->SetKd(.1);
	dlo->SetKts(11.5);
	dlo->SetKtd(.12);
	dlo->SetKsw(0.1);
	dlo->SetIntegrationMethod(IPhysicalObject::IntegrationMethod::MIDPOINT);

	this->objects.push_back(dlo);

	camPos = vec3<Real>(100,0,50);
	lookAt = vec3<Real>(0,0,0);
	upVector = vec3<Real>(0,0,1);
}

 void World::Render()
{
	glClearColor (0.0,0.0,0.0,1.0);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(camPos.x, camPos.y, camPos.z, lookAt.x, lookAt.y, lookAt.z, upVector.x, upVector.y, upVector.z);

	glColor3f(1,0,0);


	for (vector<IPhysicalObject*>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		(*it)->Render();
	}

	glutSwapBuffers();
}

 void World::Reshape(int w, int h)
{
	glViewport (0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective (60, (GLfloat)w / (GLfloat)h, .1, 1000.0);
	glMatrixMode (GL_MODELVIEW);

}

 void World::Keyboard(unsigned char key, int x, int y)
{
	if (key == 27)
	{
		exit(1);
	}


	if (key == 'q')
	{
		camPos += vec3<Real>(0,0,1);
		lookAt += vec3<Real>(0,0,1);
	}
	if (key == 'e')
	{
		camPos += vec3<Real>(0,0,-1);
		lookAt += vec3<Real>(0,0,-1);
	}
	if (key == 'w')
	{
		camPos += vec3<Real>(-1,0,0);
		lookAt += vec3<Real>(-1,0,0);
	}
	if (key == 's')
	{
		camPos += vec3<Real>(1,0,0);
		lookAt += vec3<Real>(1,0,0);
	}
	if (key == 'a')
	{
		camPos += vec3<Real>(0,-1,0);
		lookAt += vec3<Real>(0,-1,0);
	}
	if (key == 'd')
	{
		camPos += vec3<Real>(0,1,0);
		lookAt += vec3<Real>(0,1,0);
	}
	if (key == 'p')
	{
		objects[0]->Perturb(vec3<Real>(0.,0.,0.1));
	}
	if (key == 'u')
	{
		objects[0]->Update();
	}

}
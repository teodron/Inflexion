#ifndef WORLD_H
#define WORLD_H
#include "stdafx.h"
#include "GL\glew.h"
#include "GL\freeglut.h"

#include "IPhysicalObject.h"
#include "ShaderUtilities.h"

class World
{
private:
	vector<IPhysicalObject*> objects;
	vec3<Real> camPos, lookAt, upVector;

	void Init(int argc, char** argv);

	shared_ptr<FXZProgram> mainShaderProgram;

public:
	void Render();
	void Reshape(int w, int h);
	void Keyboard(unsigned char key, int x, int y);
	World(int argc, char** argv)
	{
		Init(argc, argv);
	}
	~World()
	{
		for (vector<IPhysicalObject*>::iterator it = objects.begin(); it != objects.end(); ++it)
		{
			delete *it;
		}
		objects.clear();
	}
};
#endif
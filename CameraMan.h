//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include "Camera.h"
#include "CameraNode.h"

// Singleton
class CameraMan
{
public:
	static void Add(Camera::Name name, Camera *pCamera);
	static Camera * Find( Camera::Name name);
	static void SetCurrent( Camera::Name name, Camera::Type type);
	static Camera * GetCurrent(Camera::Type type);



	static void Move3dF();
	static void Move3dB();
	static void Rotate3dU();
	static void Rotate3dD();
	static void Rotate3dL();
	static void Rotate3dR();


private:  // methods

	static CameraMan *privGetInstance();
	CameraMan();
	void privAddToFront(CameraLink *node, CameraLink *&head);


private:  // add

	CameraLink *active;
	//Camera *currCamera;
	Camera *pCamOrthographic;
	Camera *pCamPerspective;
};

#endif

//--- End of File -------------------------------------------------------------

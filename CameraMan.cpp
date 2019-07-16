//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include <assert.h>

#include "Camera.h"
#include "CameraNode.h"
#include "CameraMan.h"

CameraMan::CameraMan()
{
	this->active = 0;
	this->pCamOrthographic = 0;
	this->pCamPerspective = 0;
}

void CameraMan::Move3dF()
{
	CameraMan* manager = CameraMan::privGetInstance();
	manager->GetCurrent(Camera::Type::PERSPECTIVE_3D)->MoveForward();
}

void CameraMan::Move3dB()
{
	CameraMan* manager = CameraMan::privGetInstance();
	manager->GetCurrent(Camera::Type::PERSPECTIVE_3D)->MoveBack();
}

void CameraMan::Rotate3dU()
{
	CameraMan* manager = CameraMan::privGetInstance();
	manager->GetCurrent(Camera::Type::PERSPECTIVE_3D)->RotateUp();

}

void CameraMan::Rotate3dD()
{
	CameraMan* manager = CameraMan::privGetInstance();
	manager->GetCurrent(Camera::Type::PERSPECTIVE_3D)->RotateDown();
}

void CameraMan::Rotate3dL()
{
	CameraMan* manager = CameraMan::privGetInstance();
	manager->GetCurrent(Camera::Type::PERSPECTIVE_3D)->RotateLeft();
}

void CameraMan::Rotate3dR()
{
	CameraMan* manager = CameraMan::privGetInstance();
	manager->GetCurrent(Camera::Type::PERSPECTIVE_3D)->RotateRight();
}

CameraMan * CameraMan::privGetInstance()
{
	// This is where its actually stored (BSS section)
	static CameraMan cameraMan;
	return &cameraMan;
}

void CameraMan::Add(Camera::Name name, Camera *pCamera)
{
	// Get the instance to the manager
	CameraMan *pCameraMan = CameraMan::privGetInstance();
	assert(pCameraMan);

	// Create a TextureNode
	CameraNode *pNode = new CameraNode();
	assert(pNode);

	// initialize it
	assert(pCamera);
	pCamera->setName(name);
	pNode->set( pCamera );

	// Now add it to the manager
	pCameraMan->privAddToFront( pNode, pCameraMan->active );
}

Camera * CameraMan::Find( Camera::Name _name)
{
	// Get the instance to the manager
	CameraMan *pCameraMan = CameraMan::privGetInstance();
	assert(pCameraMan);

	CameraNode *pNode = (CameraNode *)pCameraMan->active;
	while( pNode != 0 )
	{
		if( pNode->pCamera->getName() == _name )
		{
			// found it
			break;
		}

		pNode = (CameraNode *)pNode->next;
	}
	assert(pNode);
	return pNode->pCamera;
}

void CameraMan::SetCurrent( const Camera::Name name, Camera::Type type )
{
	CameraMan *pCamMan = CameraMan::privGetInstance();

	Camera *pCam = CameraMan::Find(name);
	assert(pCam);

	switch (type)
	{
	case Camera::Type::PERSPECTIVE_3D:
		assert(pCam->getType() == Camera::Type::PERSPECTIVE_3D);
		pCamMan->pCamPerspective = pCam;
		break;

	case Camera::Type::ORTHOGRAPHIC_2D:
		assert(pCam->getType() == Camera::Type::ORTHOGRAPHIC_2D);
		pCamMan->pCamOrthographic = pCam;
		break;

	default:
		assert(false);
		break;
	}
}

Camera * CameraMan::GetCurrent(Camera::Type type )
{
	Camera *pCam = 0;
	switch (type)
	{
	case Camera::Type::PERSPECTIVE_3D:
		pCam = CameraMan::privGetInstance()->pCamPerspective;
		break;

	case Camera::Type::ORTHOGRAPHIC_2D:
		pCam = CameraMan::privGetInstance()->pCamOrthographic;
		break;

	default:
		assert(false);
		break;
	}

	assert(pCam != 0);
	return pCam;
}



void CameraMan::privAddToFront(CameraLink *node, CameraLink *&head)
{
    assert(node != 0);

    if (head == 0)
    {
        head = node;
        node->next = 0;
        node->prev = 0;
    }
    else
    {
        node->next = head;
        head->prev = node;
        head = node;
    }
}

//--- End of File -------------------------------------------------------------

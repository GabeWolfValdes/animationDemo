//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include <assert.h>

#include "CameraNode.h"
#include "Camera.h"

CameraNode::CameraNode( )
{
	this->pCamera = 0;
}

void CameraNode::set( Camera *camera )
{
	assert(camera);
	this->pCamera = camera;
}

//--- End of File -------------------------------------------------------------

//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAME_OBJECT_MAN_H
#define GAME_OBJECT_MAN_H

#include "GameObject.h"
#include "PCSTree.h"



// Singleton
class GameObjectMan
{
public :
	
	enum ObjType {
	ANIMATION,
	SKIN,
	FLAT,
	NONANIM	
	};


public:
	static void Add(GameObject *pObj, GameObject *pParent);
	static void Add(GameObject * pObj, ObjType type);
	static void Draw( void );
	static void Update(Time currentTime);
	static GameObject *GetRoot(void);

	static GameObject *GetRoot(ObjType type);


	static PCSTree *GetPCSTree();

	static GameObject* findIndex(int index);
	static GameObject * findName(char * name);
private:
	GameObjectMan();

	static GameObjectMan *privGetInstance();

// data
	PCSTree *pRootTree;
	PCSNode *pAnimRoot;
	PCSNode *p3dRoot;
	PCSNode *p2dRoot;
	PCSNode *pSkinRoot;


	int count = 0;
};

#endif

//--- End of File ------------------------------------------------------------

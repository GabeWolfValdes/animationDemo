//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include <assert.h>

#include "NullModel.h"
#include "GraphicsObject_Null.h"
#include "ShaderObject.h"

#include "GameObjectAnim.h"
#include "GameObjectMan.h"
#include "GameObjectRigid.h"
#include "PCSTreeForwardIterator.h"

void GameObjectMan::Add(GameObject *pObj, GameObject *pParent)
{
	assert(pObj != 0);
	assert(pParent != 0);

	// Get singleton
	GameObjectMan *pGOM = GameObjectMan::privGetInstance();

	// insert object to root
	pGOM->pRootTree->Insert(pObj, pParent);
}


void GameObjectMan::Add(GameObject *pObj, ObjType type) {
	GameObjectMan *pGOM = GameObjectMan::privGetInstance();

	GameObject* parent;

	switch (type) {
		case ObjType::ANIMATION:
			parent = pGOM->GetRoot(type);
			break;	
		case ObjType::SKIN:
			parent = pGOM->GetRoot(type);
			break;	
		case ObjType::FLAT:
			parent = pGOM->GetRoot(type);
			break;		
		case ObjType::NONANIM:	
				parent = pGOM->GetRoot(type);
				break;
		default:
			parent = 0;
			assert(0);
			break;
	}
	// insert object to root
	pGOM->pRootTree->Insert(pObj, parent);

}



PCSTree *GameObjectMan::GetPCSTree()
{
	// Get singleton
	GameObjectMan *pGOM = GameObjectMan::privGetInstance();
	assert(pGOM);

	// Get root node
	return pGOM->pRootTree;
}

GameObject * GameObjectMan::findIndex(int index)
{
	GameObjectMan* manager = GameObjectMan::privGetInstance();

	
	GameObjectAnim* temp;
	GameObject* result = nullptr;

	PCSTreeForwardIterator itr(manager->GetRoot(ObjType::ANIMATION));
	PCSNode* node = itr.First();

	while (node != 0) {
		temp = (GameObjectAnim*)node;
		if (temp->GetIndex() == index) {
			result = temp;
			break;
		}
		node = itr.Next();
	}


	return result;
}

GameObject * GameObjectMan::findName(char* name)
{
	GameObjectMan* manager = GameObjectMan::privGetInstance();


	GameObject* temp;
	GameObject* result = nullptr;

	PCSTreeForwardIterator itr(manager->GetRoot());
	PCSNode* node = itr.First();

	char* tmpName = new char[PCSNode::NAME_SIZE];

	while (node != 0) {
		temp = (GameObject*)node;
		temp->GetName(tmpName,temp->NAME_SIZE);
		if (strcmp(name,tmpName)==0) {
			result = temp;
			break;
		}
		node = itr.Next();
	}


	return result;
}



GameObject * GameObjectMan::GetRoot()
{
	// Get singleton
	GameObjectMan *pGOM = GameObjectMan::privGetInstance();
	assert(pGOM);

	GameObject *pGameObj = (GameObject *)pGOM->pRootTree->GetRoot();
	assert(pGameObj);

	return pGameObj;
}

GameObject * GameObjectMan::GetRoot(ObjType type)
{
	GameObjectMan *pGOM = GameObjectMan::privGetInstance();
	assert(pGOM);

	GameObject *pGameObj;
	
	switch (type) {
		case ObjType::ANIMATION:
			pGameObj = (GameObject *)pGOM->pAnimRoot;
		break;		
		case ObjType::SKIN:	
			pGameObj = (GameObject *)pGOM->pSkinRoot;
			break;
		case ObjType::FLAT:
			pGameObj = (GameObject *)pGOM->p2dRoot;
			break;
		case ObjType::NONANIM:
			pGameObj = (GameObject *)pGOM->p3dRoot;
			break;
		default:
			pGameObj = 0;
			assert(0);
			break;
	}

	return pGameObj;
}

void GameObjectMan::Update(Time currentTime)
{
	GameObjectMan *pGOM = GameObjectMan::privGetInstance();
	assert(pGOM);

	PCSNode *pRootNode = pGOM->pRootTree->GetRoot();
	assert(pRootNode);

	//PCSTreeForwardIterator pIter(pRootNode->GetChild());

	//PCSNode *pNode = pIter.First();

	//GameObject *pGameObj = 0;

	//while (!pIter.IsDone())
	//{
	//	assert(pNode);
	//	// Update the game object
	//	pGameObj = (GameObject *)pNode;
	//	pGameObj->Update(currentTime);

	//	pNode = pIter.Next();
	//}

	//changed this to make the root the beginning
	//PCSTreeForwardIterator pIter(pRootNode->GetChild());
	PCSTreeForwardIterator pIter(pRootNode);
	//pIter.Next();
	PCSNode *pNode = pIter.First();
	
	GameObject *pGameObj = 0;

	while (!pIter.IsDone())
	{
		assert(pNode);
		// Update the game object
		pGameObj = (GameObject *)pNode;
		pGameObj->Update(currentTime);

		pNode = pIter.Next();
	}

	// Skeleton 2
	//PCSTreeForwardIterator pIter2(pRootNode->GetChild()->GetNextSibling());
	//PCSNode *pNode2 = pIter2.First();
	//GameObject *pGameObj2 = 0;


	//while (!pIter2.IsDone())
	//{
	//	assert(pNode2);
	//	// Update the game object
	//	pGameObj2 = (GameObject *)pNode2;
	//	pGameObj2->Update(currentTime);

	//	pNode2 = pIter2.Next();
	//}

}

void GameObjectMan::Draw()
{
	GameObjectMan *pGOM = GameObjectMan::privGetInstance();
	assert(pGOM);

	PCSNode *pRootNode = pGOM->pRootTree->GetRoot();
	assert(pRootNode);

	PCSTreeForwardIterator pForwardIter(pRootNode);
	PCSNode *pNode = pForwardIter.First();

	GameObject *pGameObj = 0;
	//int count = 0;

	while (!pForwardIter.IsDone())
	{
		assert(pNode);
		// Update the game object
		pGameObj = (GameObject *)pNode;

		//PA1
		if (pGameObj->GetDrawEnable())
		{
			pGameObj->Draw();
		}


		//remove later for testing ONLY
		//remove later for testing ONLY
		//char* tmpName = new char[PCSNode::NAME_SIZE];
		//pGameObj->GetName(tmpName, pGameObj->NAME_SIZE);
		//if (strcmp("HumanSkin", tmpName) == 0) {
		//	pGameObj->Draw();
		//	break;
		//}


		//pGameObj->Draw();
		//else {
		//	pGOM->count++;
		//}
		pNode = pForwardIter.Next();
	}
}

GameObjectMan::GameObjectMan( )
{
	// Create the root node (null object)
	NullModel *pModel = new NullModel(0);
	ShaderObject *pShader = new ShaderObject(ShaderObject::Name::NULL_SHADER, "nullRender");
	GraphicsObject_Null *pGraphicsObject = new GraphicsObject_Null(pModel, pShader);
	
	GameObjectRigid *pGameRoot = new GameObjectRigid(pGraphicsObject);
	pGameRoot->SetName("GameObject_Root");

	GameObjectRigid *pAnimGameRoot = new GameObjectRigid(pGraphicsObject);
	pAnimGameRoot->SetName("Animation_Root");


	GameObjectRigid *p3dGameRoot = new GameObjectRigid(pGraphicsObject);
	p3dGameRoot->SetName("3dGameObject_Root");

	GameObjectRigid *p2dGameRoot = new GameObjectRigid(pGraphicsObject);
	p2dGameRoot->SetName("2dGameObject_Root");

	GameObjectRigid *pSkinGameRoot = new GameObjectRigid(pGraphicsObject);
	p2dGameRoot->SetName("2dGameObject_Root");

	// Create the tree
	this->pRootTree = new PCSTree();
	assert(this->pRootTree);

	this->pAnimRoot = pAnimGameRoot;
	this->p2dRoot = p2dGameRoot;
	this->pSkinRoot = pSkinGameRoot;
	this->p3dRoot = p3dGameRoot;

	

	// Attach the root node
	this->pRootTree->Insert(pGameRoot, this->pRootTree->GetRoot());
	this->pRootTree->Insert(pAnimGameRoot, this->pRootTree->GetRoot());
	this->pRootTree->Insert(p2dGameRoot, this->pRootTree->GetRoot());
	this->pRootTree->Insert(p3dGameRoot, this->pRootTree->GetRoot());
	this->pRootTree->Insert(pSkinGameRoot, this->pRootTree->GetRoot());
	

}
	
GameObjectMan * GameObjectMan::privGetInstance( void )
{
	// This is where its actually stored (BSS section)
	static GameObjectMan gom;
	return &gom;
}

//--- End of File ------------------------------------------------------------


// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include <assert.h>
#include <d3dx9.h>
#include <vector>
#include <list>
#include <map>
#include <set>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#define DRAW_GRID
#define TEST_1
//#define TEST_2


extern HWND g_hWnd;

#define SAFE_ADD(p) { if(p) p->AddRef(); }
#define SAFE_DELETE(p) { if(p) { delete p; p = NULL; } }
#define SAFE_RELEASE(p) { if(p) { p->Release(); p = NULL; } }

#define SYNTHESIZE(varType, varName, funName) \
protected: varType varName;\
public: inline varType Get##funName(void) const { return varName; }\
public: inline void Set##funName(varType var) { varName = var; }

#define SYNTHESIZE_REF(varType, varName, funName) \
protected: varType varName;\
public: inline varType& Get##funName(void) { return varName; }\
public: inline void Set##funName(varType var) { varName = var; }

#define SYNTHESIZE_REF_16(varType, varName, funName) \
protected: varType varName;\
public: inline varType& Get##funName(void) { return varName; }\
public: inline void Set##funName(varType &var) { varName = var; }


#define SYNTHESIZE_ADD_REF(varType, varName, funName) \
protected: varType varName;\
public: virtual varType Get##funName(void) const { return varName; }\
public: virtual void Set##funName(varType var)\
{\
	if (varName != var)\
	{\
		SAFE_ADD(var);\
		SAFE_RELEASE(varName);\
		varName = var;\
	}\
}

#define SINGLETONE(class_name) \
	private: \
		class_name(void); \
		~class_name(void); \
	public: \
		static class_name* GetInstance() \
		{ \
			static class_name instance; \
			return &instance; \
		}

struct ST_PC_VERTEX
{
	D3DXVECTOR3 p;
	D3DCOLOR	c;
	enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
};

struct ST_PT_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR2	t;
	enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1 };
};

struct ST_PTN_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;
	D3DXVECTOR2	t;
	enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_NORMAL };
};

struct ST_POS_SAMPLE
{
	int			frame;
	D3DXVECTOR3 v;

	ST_POS_SAMPLE() : frame(0), v(0, 0, 0) {}
};

struct ST_ROT_SAMPLE
{
	int				frame;
	D3DXQUATERNION	q;

	ST_ROT_SAMPLE() : frame(0)
	{
		D3DXQuaternionIdentity(&q);
	}
};


/* Manager */
#include "cDeviceManager.h"
#include "cTimeManager.h"
#include "cTextureManager.h"
#include "cFontManager.h"
#include "cSceneManager.h"

/* UTIL */
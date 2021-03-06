// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <memory>
#include <functional>

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include <bitset>
#include <assert.h>
#include <d3dx9.h>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>

#include <iostream>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")


#define DRAW_GRID
#define TEST_1
//#define TEST_2
#define WINNAME (LPTSTR)(TEXT("Orcs Must Die"))	//윈도우 창 이름
#define WINSTARTX 20						//윈도우 시작좌표X
#define WINSTARTY 20						//윈도우 시작좌표Y
#define WINSIZEX 1280						//윈도우 가로크기
#define WINSIZEY 800						//윈도우 세로크기
#define WINSTYLE WS_CAPTION | WS_SYSMENU

/* Extern */
extern HWND g_hWnd;
extern POINT g_ptMouse;
extern bool g_bIsClientActivated;

/* Macro Function List */
#define SAFE_ADD(p) { if(p) p->AddRef(); }
#define SAFE_DELETE(p) { if(p) { delete p; p = NULL; } }
#define SAFE_DELETE_ARRAY(p) {if(p) { delete[](p); (p) = NULL;}}
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

/* Public Struct List*/
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
/* Public Enum List */
#include "GameUIEnumList.h"

/* Manager */
#include "cDeviceManager.h"
#include "cTimeManager.h"
#include "cTextureManager.h"
#include "cFontManager.h"
#include "cSceneManager.h"
#include "cKeyManager.h"
#include "cSoundManager.h"
#include "cFontManager.h"
#include "cParticleManager.h"
#include "cXFileManager.h"

/* Utils*/
#include "commonMacroFunction.h"
#include "utils.h"
#include "additionalUtils.h"
#include "D3DX_UTIL.h"
using namespace THETA_UTIL;
using namespace D3DX_UTIL;
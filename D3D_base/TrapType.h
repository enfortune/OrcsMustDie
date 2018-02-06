#pragma once

// conceptual class

class Trap;
struct TrapTypeComponentAttackable;
struct TrapTypeComponentBlockable;
struct TrapTypeComponentTriggerable;

#include "cSkinnedMeshEX.h"

struct TrapType
{
public:
	// '|' operation
	enum class eInstallPosition { FLOOR = 0, WALL, CEILING, MAX_COUNT };
	enum class eInteractionTo { ENEMY = 0, PLAYER, ALLY, TRIGGER, MAX_COUNT };

	enum class eTimerType { NONE, COOLDOWN, REPAIR, ALWAYS };

protected:
	std::vector<cSkinnedMeshEX> modelList_;
	D3DXMATRIXA16 matrixLocal_ {};

	int width_ {};
	int height_ {};

	FRUSTUM frustumLocal_ {};
	bool isBlockable_ {};

	FRUSTUM frustumInteractionLocal_ {};

	std::bitset<static_cast<size_t> (eInstallPosition::MAX_COUNT)> isInstallPositionArray_ {};
	std::bitset<static_cast<size_t> (eInteractionTo::MAX_COUNT)> isInteractionToArray_ {};

	eTimerType timerType_ {};

	TrapTypeComponentAttackable * pTypeComponentAttackable_ {};
	TrapTypeComponentBlockable * pTypeComponentBlockable_ {};
	TrapTypeComponentTriggerable * pTypeComponentTriggerable_ {};
	
	//std::vector<LPDIRECT3DTEXTURE9> pTextureList_ {};
	//std::vector<std::string> soundList_ {};

private:
	void render(D3DXMATRIXA16 & matrixWorld, int modelIndex);		// inner model index of specific trap
	
public:
	TrapType() = default;
	virtual ~TrapType();

	//UNCOPYABLE(TrapType);

	friend Trap;

public:
	inline bool isConstructible(TrapType::eInstallPosition installPosition) const
	{ return isInstallPositionArray_[static_cast<int> (installPosition)]; }

	inline bool isBlockable() const { return isBlockable_; }

	inline int getWidth() const { return width_; }
	inline int getHeight() const { return height_; }

	inline FRUSTUM getFrustum() const { return frustumLocal_; }
	inline FRUSTUM getInteractionArea() const { return frustumInteractionLocal_; }

//public:
	//void onTrigger();

public:
	void render(D3DXMATRIXA16 & matrixWorld);	// rendering virtual object
};
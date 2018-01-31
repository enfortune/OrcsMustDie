#pragma once

// conceptual class

class Trap;
class TrapTypeComponentAttackable;
class TrapTypeComponentBlockable;
class TrapTypeComponentTriggerable;

class cSkinnedMesh;

struct TrapType
{
public:
	// '|' operation
	enum class eInstallPosition { FLOOR = 0, WALL, CEILING, MAX_COUNT };
	enum class eInteractionTo { ENEMY = 0, PLAYER, ALLY, TRIGGER, MAX_COUNT };

	enum class eTimerType { NONE, COOLDOWN, REPAIR, ALWAYS };

protected:
	cSkinnedMesh * pModel_;

	FRUSTUM frustumLocal_ {};
	bool isBlockable_ {};

	FRUSTUM frustumInteractionLocal_ {};

	std::bitset<static_cast<size_t> (eInstallPosition::MAX_COUNT)> isInstallPositionArray_ {};
	std::bitset<static_cast<size_t> (eInteractionTo::MAX_COUNT)> isInteractionToArray_ {};

	eTimerType timerType_ {};

	TrapTypeComponentAttackable * pComponentAttackable_ {};
	TrapTypeComponentBlockable * pComponentBlockable_ {};
	TrapTypeComponentTriggerable * pComponentTriggerable_ {};
	
	//std::vector<LPDIRECT3DTEXTURE9> pTextureList_ {};
	//std::vector<std::string> soundList_ {};
	
public:
	TrapType() = default;
	virtual ~TrapType() = default;

	//UNCOPYABLE(TrapType);

	friend Trap;

public:
	inline FRUSTUM getFrustum() const { return frustumLocal_; }

//public:
	//void onTrigger();

public:
	void render(D3DXMATRIXA16 & matrixWorld);
};
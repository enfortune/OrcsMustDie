#include "stdafx.h"
#include "cEnemyBase.h"


cEnemyBase::cEnemyBase()
{

	g_pSoundManager->AddSound("WolfBite", "Resource/Sound/Enemy/BiteMediumB.ogg", false, false);
	g_pSoundManager->AddSound("OrcAttack", "Resource/Sound/Enemy/1h_Sword_Hit_Armor_Plate_03.ogg", false, false);
	g_pSoundManager->AddSound("MountAttack", "Resource/Sound/Enemy/MountainGiantAttackA.ogg", false, false);
	g_pSoundManager->AddSound("MountSkill", "Resource/Sound/Enemy/MountainGiantAttackB.ogg", false, false);
	g_pSoundManager->AddSound("MountDead", "Resource/Sound/Enemy/MountainGiantDeath.ogg", false, false);
	g_pSoundManager->AddSound("ShockWave", "Resource/Sound/Enemy/Cannon01_BlastA.ogg", false, false);

}


cEnemyBase::~cEnemyBase()
{
}

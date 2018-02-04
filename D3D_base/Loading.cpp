#include "stdafx.h"
#include "Loading.h"

#pragma warning(disable : 4996)

#include "cSkinnedMeshEX.h"

int Loading::getMaxLoadCount() { return static_cast<int> (textureTable_.size() + modelTable_.size() + soundTable_.size()); }

void Loading::addData(TextureData texture) { textureTable_.emplace(texture.filePath, false); }
void Loading::addData(ModelData model) { modelTable_.emplace(std::make_pair(model.folderPath, model.fileName), false); }
void Loading::addData(SoundData sound) { soundTable_.emplace(sound.key, std::make_pair(sound.filePath, false)); }

bool Loading::load(OUT std::vector<cSkinnedMeshEX> & skinnedMeshList)
{
	for (auto iter = textureTable_.begin(); iter != textureTable_.end(); ++iter)
	{
		if (iter->second == false)
		{
			assert(g_pTextureManager->GetTexture(iter->first));
			
			iter->second = true;
			++loadCount_;

			if (loadCount_ == getMaxLoadCount())
				return true;
			else
				return false;
		}
	}

	for (auto iter = modelTable_.begin(); iter != modelTable_.end(); ++iter)
	{
		if (iter->second == false)
		{
			char * pFolderPath = new char[iter->first.first.size()];
			char * pFileName = new char[iter->first.second.size()];

			strcpy(pFolderPath, iter->first.first.c_str());
			strcpy(pFileName, iter->first.second.c_str());

			skinnedMeshList.emplace_back();
			skinnedMeshList[skinnedMeshList.size() - 1].Setup(pFolderPath, pFileName);

			delete pFolderPath;
			delete pFileName;

			++loadCount_;

			if (loadCount_ == getMaxLoadCount())
				return true;
			else
				return false;
		}
	}

	for (auto iter = soundTable_.begin(); iter != soundTable_.end(); ++iter)
	{
		if (iter->second.second == false)
		{
			//g_pSoundManager->AddSound()
			++loadCount_;

			if (loadCount_ == getMaxLoadCount())
				return true;
			else
				return false;
		}
	}

	return false;
}
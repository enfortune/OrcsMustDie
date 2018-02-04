#pragma once

class cSkinnedMeshEX;

class Loading
{
public:
	struct TextureData
	{
		std::string filePath {};
	};

	struct ModelData
	{
		std::string folderPath {};
		std::string fileName {};
	};

	struct SoundData
	{
		std::string key {};
		std::string filePath {};
	};

private:
	std::map<std::string, bool> textureTable_;
	std::map<std::pair<std::string, std::string>, bool> modelTable_;
	std::map<std::string, std::pair<std::string, bool> > soundTable_;

	int loadCount_ {};

public:
	inline int getCurrentLoadCount() const { return loadCount_; }
	int getMaxLoadCount();

public:
	void addData(TextureData texture);
	void addData(ModelData model);
	void addData(SoundData sound);

	bool load(OUT std::vector<cSkinnedMeshEX> & skinnedMeshList);
};
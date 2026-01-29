#pragma once
#include "Object.h"
#include <unordered_map>
#include <filesystem>
#include <future>
#include "Asset.h"

class AssetManager : public Object
{
	class AssetRegistery {
	private:
		std::unordered_map<std::filesystem::path, uint64_t> path2uuid{};
		std::unordered_map<uint64_t, std::filesystem::path> uuid2path{};
	public:
		void addRecord(uint64_t uuid, std::filesystem::path path);
		void removeRecord(uint64_t uuid);
		void removeRecord(std::filesystem::path path);

		void importRegistery(const YAML::Node& node);
		void exportRegistery(YAML::Emitter& out);

		uint64_t operator()(std::filesystem::path path); // 0 if not exits 
		std::filesystem::path operator()(uint64_t uuid);

		void clear(); // clear all records. Maybe unneccessary 
	}AR;

	std::unordered_map<uint64_t, std::shared_ptr<Asset>> _assets; // Tüm assetler
	std::vector<std::future<void>> _activeLoads; // Async ile CPU da işlem görenler 
	std::vector<std::shared_ptr<Asset>> _pendingUploads; // Async işlemi sonrası GPU ya yüklenmeyi bekleyenler 

	//void load(std::filesystem::path path, const IAssetSettings* settings = nullptr);
	//void loadAsync(std::filesystem::path path, const IAssetSettings* settings = nullptr);
public:

	template <class T>
	std::shared_ptr<T> get(uint64_t id);

	template <class T>
	std::shared_ptr<T> get(std::filesystem::path path, const IAssetSettings* settings = nullptr, bool async = false);

	void update();

	// apply serialize and deserialize
};





template<class T>
inline std::shared_ptr<T> AssetManager::get(uint64_t id)
{
	// Acaba buradaki complete statüsün sorgulamamız yanlış mı? 
	//if (_assets.find(id) != _assets.end() && _assets[id]->getLoadStatus == AssetLoadStatus::Complete) 
	if (_assets.find(id) != _assets.end()) 
		return std::dynamic_pointer_cast<T>(_assets[id]);

	return std::shared_ptr<T>();

}

template<class T>
inline std::shared_ptr<T> AssetManager::get(std::filesystem::path path, const IAssetSettings* settings, bool async)
{
	// Asset varsa olanı döndür
	if (uint64_t assetID = AR(path)) 
		return get<T>(assetID);

	// Yoksa yükle

	std::shared_ptr<Asset> asset = std::make_shared<T>();
	_assets[asset->UUID] = asset; 

	AR.addRecord(asset->UUID, path);

	if (async) {
		_activeLoads.push_back(std::async(std::launch::async, &Asset::load, asset.get(), path, *settings));
		_pendingUploads.push_back(asset);
	}
	else {
		asset->load(path, *settings);
		asset->uploadToGPU();
	}

	return get<T>(asset->UUID); // yeni assetin id sini sorgula

}


inline AssetManager g_Assets{};
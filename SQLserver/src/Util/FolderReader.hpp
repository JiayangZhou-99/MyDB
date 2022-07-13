//
//  FolderReader.hpp
// 

#ifndef FolderReader_hpp
#define FolderReader_hpp

#include <filesystem>
#include <functional>
#include "Config.hpp"

namespace fs = std::filesystem;

namespace MyDB {

	using FileVisitor = std::function<bool(const std::string&)>;

	class FolderReader {
	public:
		explicit FolderReader(const char* aPath) : path(aPath) {}
		explicit FolderReader(const std::string& aPath) : path(aPath) {}

		bool exists(const std::string& aFilename) const {
			return fs::exists(Config::getDBPath(aFilename));
		}

		void each(const std::string& anExt, const FileVisitor& aVisitor) const {
			for (auto& thePath : std::filesystem::directory_iterator(path)) {
				if (thePath.is_regular_file() 
					&& thePath.path().extension().string() == anExt) {
					aVisitor(thePath.path().string());
				}
			}
		};

		std::string path;
	};
}

#endif /* FolderReader_h */

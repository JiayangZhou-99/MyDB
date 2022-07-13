//
//  FolderView.hpp
//
//

#ifndef FolderView_h
#define FolderView_h

#include "FolderReader.hpp"
#include "View.hpp"
#include "TableView.hpp"
#include <iomanip>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

namespace MyDB {

	// USE: provide view class that lists db files in storage path...
	class FolderView : public View {
	public:
		FolderView(std::string const& aPath, std::string const& anExtension = ".db")
			: reader(aPath), extension(anExtension) {}

		bool show(std::ostream& anOutput) override {
			auto theTableView = TableView("Database Files");
            
			reader.each(extension, [&](const std::string& aName) {
				theTableView.addData(fs::path(aName).stem().string());
				return true;
				});

			theTableView.show(anOutput);
			return true;
		}

		FolderReader  reader;
		std::string extension;
	};

}

#endif /* FolderView_h */

#ifndef TABLEVIEW_HPP
#define TABLEVIEW_HPP

#include <vector>
#include <sstream>
#include "View.hpp"
#include "keywords.hpp"

namespace MyDB {
	class TableView : public View
	{
	public:

		/// <summary>
		/// Creates a table view. Arguments are the column names
		/// Example: TableView("Col1", "Col2", "Col3") will create a
		/// TableView with 3 columns of the above names
		/// </summary>
		template <typename ... T>
		TableView(T && ... aVarColNames);
        TableView(std::vector<std::string> aTableHeader);

		bool show(std::ostream& aStream) override;

		/// <summary>
		/// Add a single row of data to the table view. Each piece of data
		/// should be a new argument.
		/// </summary>
		template <typename ... T>
		void addData(T && ... aVarData);
        void addData(std::vector<std::string> aVarData);

	protected:
		/// <summary>
		/// Draws a formatted row separator for a table
		/// </summary>
		/// <param name="aStream">The stream to output to</param>
		void drawRowSeparator(std::ostream& aStream) const;

		/// <summary>
		/// Draws a row of data formatted with separators for a table
		/// </summary>
		/// <param name="aStream">The stream to output to</param>
		/// <param name="aStartIndex">The index of the row of data</param>
		void drawRow(std::ostream& aStream, size_t aStartIndex) const;

		std::vector<std::string> data;
		std::vector<size_t> columnWidths;

		size_t numCols;

		const static char cornerChar = '+';
		const static char rowChar = '-';
		const static char colChar = '|';
		const static size_t kExtraPadding = 3;
	};
    
    inline TableView::TableView(std::vector<std::string> aTableHeader){
        for(auto theData:aTableHeader){
            data.push_back(theData);
            columnWidths.push_back(theData.size());
        }
        numCols = data.size();
    }

    inline void TableView::addData(std::vector<std::string> aVarData){
        
        size_t theIndex = 0;
        
        for(auto theData:aVarData){
            data.push_back(theData);
            size_t& theColWidthRef = columnWidths.at(theIndex++ % numCols);
            if (theColWidthRef < theData.size())
            {
                theColWidthRef = theData.size();
            }
        }
        
    }


	template<typename ...T>
	inline TableView::TableView(T && ...aVarColNames)
	{
		//Unpack the variadic input
		([&](const auto& aColName)
			{
				//Using a stringstream to convert arbitrary types to a string
				std::stringstream theStream;
				theStream << aColName;

				data.push_back(theStream.str());

				//Since this is the first data to go into the table, the size
				//of the column is the length of the string
				columnWidths.push_back(theStream.str().size());
			} (aVarColNames), ...);
		numCols = data.size();
	}

	template<typename ...T>
	inline void TableView::addData(T && ...aVarData)
	{
		size_t theIndex = 0;
		([&](const auto& aData)
			{
				//Using a stringstream to convert any type to string assuming
				//it implements the stream operator
				std::stringstream theStream;
				theStream << aData;
				data.push_back(theStream.str());

				//Updating the column width if the current size is smaller than the new data
				size_t& theColWidthRef = columnWidths.at(theIndex++ % numCols);
				if (theColWidthRef < theStream.str().size())
				{
					theColWidthRef = theStream.str().size();
				}
			} (aVarData), ...);
	}
}

#endif // !TABLEVIEW_HPP

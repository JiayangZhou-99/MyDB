#include "TableView.hpp"
#include <iomanip>
#include "keywords.hpp"
#include "TimerView.hpp"

namespace MyDB {
	bool TableView::show(std::ostream& aStream)
	{
		size_t theNumRows = data.size() / numCols;

		//Table Header Row Separator
		drawRowSeparator(aStream);

		//Draw the headers and data rows
		for (size_t theRowIndex = 0; theRowIndex < theNumRows; ++theRowIndex)
		{
			drawRow(aStream, theRowIndex * numCols);
		}

		//Subtract 1 for column headers
		aStream << theNumRows - 1 << " rows in set";

		TimerView().show(aStream);

		return true;
	}

	void TableView::drawRowSeparator(std::ostream& aStream) const
	{
		aStream << cornerChar;
		for (const size_t& theColWidth : columnWidths)
		{
			aStream << std::setfill(rowChar) << std::setw(theColWidth + kExtraPadding) << "" << cornerChar;
		}
		aStream << '\n';
	}

	void TableView::drawRow(std::ostream& aStream, size_t aStartIndex) const
	{
		aStream << colChar;
		size_t theEndIndex = aStartIndex + numCols;
		for (size_t theItemIndex = aStartIndex; theItemIndex < theEndIndex; ++theItemIndex)
		{
			aStream << std::left << std::setfill(' ') 
				<< std::setw(columnWidths[theItemIndex % numCols] + kExtraPadding) 
				<< data[theItemIndex] << colChar;
		}
		aStream << '\n';
		drawRowSeparator(aStream);
	}
}

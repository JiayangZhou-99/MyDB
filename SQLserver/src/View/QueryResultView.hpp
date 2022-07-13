#ifndef QUERYRESULTVIEW_HPP
#define QUERYRESULTVIEW_HPP

#include "View.hpp"
#include "TimerView.hpp"

namespace MyDB {

	class QueryResultView : public View {
	public:
		QueryResultView(size_t aRowCount): numRowsAffected(aRowCount) {}

		bool show(std::ostream& aStream) override {
			aStream << "Query OK, " << numRowsAffected << " rows affected";
			TimerView().show(aStream);
			return true;
		}

	protected:
		size_t numRowsAffected;
	};

}

#endif // !QUERYRESULTVIEW_HPP

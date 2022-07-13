#ifndef TIMERVIEW_HPP
#define TIMERVIEW_HPP

#include "View.hpp"
#include <iomanip>
#include <string>
#include "Config.hpp"

namespace MyDB {
	class TimerView : public View {
	public:
		TimerView() {}

		bool show(std::ostream& anOutput) override {
			anOutput << " (" << std::fixed << Config::getTimer().elapsed() << " sec)" << std::endl;
			return true;
		}
	};
}

#endif /* TIMERVIEW_HPP */

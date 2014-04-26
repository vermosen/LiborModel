#ifndef curve_creation
#define curve_creation

#include <ql/quantlib.hpp>

using namespace QuantLib;

struct depositData {

	Real    quote_;
	Natural settlementDays_;
	Period  maturity_;

};

struct futureData {

	Real        price_;
	std::string futureCode_;

};

struct swapData {

	Rate   fairRate_;
	Period maturity_;

};

struct swaptionData {

	Volatility volatility_;
	Period     lenght_;
	Period     maturity_;

};

boost::shared_ptr<IborIndex> curveCreation();

#endif
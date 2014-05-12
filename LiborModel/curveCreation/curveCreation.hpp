#ifndef curve_creation
#define curve_creation

#include <ql/termstructures/yield/depositBootstrapHelper.hpp>
#include <ql/termstructures/yield/interestRateSwapBootstrapHelper.hpp>
#include <ql/termstructures/yield/ratehelpers.hpp>
#include <ql/time/daycounters/all.hpp>
#include <ql/quotes/futuresConvexityAdjustmentQuote2.hpp>
#include <ql/time/calendars/all.hpp>
#include <ql/indexes/ibor/usdlibor.hpp>
#include <ql/termstructures/yield/swapCurve.hpp>
#include <ql/utilities/csvBuilder.hpp>

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

boost::shared_ptr<IborIndex> curveCreation(utilities::csvBuilder &);

#endif
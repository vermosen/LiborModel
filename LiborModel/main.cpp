/*
 * 
 * The goal of this project is to calibrate blabla...
 *
 *
 *
 *
 *
 */

#include <ql/quantlib.hpp>
#include <ql/utilities/csvBuilder.hpp>


using namespace QuantLib;

namespace {

	/* create a libor index */
	boost::shared_ptr<IborIndex> makeIndex(std::vector<Date> dates,
		std::vector<Rate> rates) {
		DayCounter dayCounter = Actual360();

		// notice the relinkable index...
		RelinkableHandle<YieldTermStructure> termStructure;

		boost::shared_ptr<IborIndex> index(new Euribor6M(termStructure));

		Date todaysDate =
			index->fixingCalendar().adjust(Date(4, September, 2005));
		Settings::instance().evaluationDate() = todaysDate;

		dates[0] = index->fixingCalendar().advance(todaysDate,
			index->fixingDays(), Days);

		termStructure.linkTo(boost::shared_ptr<YieldTermStructure>(
			new ZeroCurve(dates, rates, dayCounter)));

		return index;
	}

	/* feed an index with data */
	boost::shared_ptr<IborIndex> makeIndex() {
		std::vector<Date> dates;
		std::vector<Rate> rates;
		dates.push_back(Date(4, September, 2005));
		dates.push_back(Date(4, September, 2018));
		rates.push_back(0.039);
		rates.push_back(0.041);

		return makeIndex(dates, rates);
	}

}

struct depositData {

	Real   quote_;
	Natural settlementDays_;
	Period maturity_;

};

struct swaptionData {

	Volatility volatility_;
	Period     maturity_;
	Period     lenght_;

};

int main() {

	std::cout << "Testing calibration of a Libor forward model..." << std::endl;

	SavedSettings backup;

	const Size size = 20;								// 20 semestrers
	const Real tolerance = 8e-3;						// tolerance

	const Calendar calendar								// pricing calendar
		= JointCalendar(
		UnitedStates(UnitedStates::Settlement),
		UnitedKingdom(UnitedKingdom::Settlement));

	const Date pricingDate = Date(16, April, 2014);		// pricing date

	QuantLib::Settings::instance().evaluationDate()		// set global evaluation date
		= pricingDate;

	std::vector<boost::shared_ptr<RateHelper> > rateHelpers;

	std::vector<depositData> rates						// yield data
		= std::vector<depositData> {
	
			{ .09070, 0, Period(1, Days) },
			{ .14250, 1, Period(2, Days) },
			{ .12150, 2, Period(1, Weeks) },
			{ .15200, 2, Period(1, Months) },
			{ .19300, 2, Period(2, Months) },
			{ .22785, 2, Period(3, Months) }
	
	};

	
	for (std::vector<depositData>::const_iterator It = rates.cbegin();
		It != rates.cend(); It++) {
	
		// settlement and maturity dates
		Date settlement = calendar.advance(
			pricingDate, Period(It->settlementDays_, Days));
		Date maturity = calendar.advance(
			settlement, It->maturity_);

		// creating the deposit
		boost::shared_ptr<QuantLib::deposit> myDepositPtr(
			new deposit(
				pricingDate,
				maturity,
				calendar,
				It->settlementDays_));

		Handle<Quote> quoteHandler(						// quote handle
			boost::shared_ptr<Quote>(
				new SimpleQuote(
					myDepositPtr->cleanPrice(
					It->quote_,
					Actual360(),
					Simple,
					Once,
					Unadjusted,
					settlement))));


		rateHelpers.push_back(							// insert RateHelper
			boost::shared_ptr<RateHelper>(new
				depositBootstrapHelper(
					quoteHandler,
					myDepositPtr)));
	
	}

	boost::shared_ptr<swapCurve> curve(					// building the curve object
		new swapCurve(rateHelpers, calendar));

	// rought check on yield curve points
	std::cout << "yield curve value on 1M: " 
			  << curve->zeroRate(1/12, Continuous)
			  << std::endl;

	std::cout << "yield curve value on 2M: "
		<< curve->zeroRate(2 / 12, Continuous)
		<< std::endl;

	std::cout << "yield curve value on 3M: "
		<< curve->zeroRate(3 / 12, Continuous)
		<< std::endl;

	std::vector<swaptionData> swaptions					// swaption data
		= std::vector<swaptionData> {
	
		{ 0.170595, Period(1, Years), Period(1, Years) },
		{ 0.166844, Period(1, Years), Period(2, Years) },
		{ 0.158306, Period(1, Years), Period(3, Years) },
		{ 0.136930, Period(1, Years), Period(4, Years) },
		{ 0.126833, Period(1, Years), Period(5, Years) },
		{ 0.118135, Period(1, Years), Period(6, Years) },
		{ 0.175963, Period(1, Years), Period(7, Years) },
		{ 0.166359, Period(1, Years), Period(8, Years) },
		{ 0.155203, Period(1, Years), Period(9, Years) },
		{ 0.170595, Period(2, Years), Period(1, Years) },
		{ 0.166844, Period(2, Years), Period(2, Years) },
		{ 0.158306, Period(2, Years), Period(3, Years) },
		{ 0.136930, Period(2, Years), Period(4, Years) },
		{ 0.126833, Period(2, Years), Period(5, Years) },
		{ 0.118135, Period(2, Years), Period(6, Years) },
		{ 0.175963, Period(2, Years), Period(7, Years) },
		{ 0.166359, Period(2, Years), Period(8, Years) },
		{ 0.170595, Period(3, Years), Period(1, Years) },
		{ 0.166844, Period(3, Years), Period(2, Years) },
		{ 0.158306, Period(3, Years), Period(3, Years) },
		{ 0.136930, Period(3, Years), Period(4, Years) },
		{ 0.126833, Period(3, Years), Period(5, Years) },
		{ 0.118135, Period(3, Years), Period(6, Years) },
		{ 0.175963, Period(3, Years), Period(7, Years) },
		{ 0.170595, Period(4, Years), Period(1, Years) },
		{ 0.166844, Period(4, Years), Period(2, Years) },
		{ 0.158306, Period(4, Years), Period(3, Years) },
		{ 0.136930, Period(4, Years), Period(4, Years) },
		{ 0.126833, Period(4, Years), Period(5, Years) },
		{ 0.118135, Period(4, Years), Period(6, Years) },
		{ 0.175963, Period(5, Years), Period(1, Years) },
		{ 0.166844, Period(5, Years), Period(2, Years) },
		{ 0.158306, Period(5, Years), Period(3, Years) },
		{ 0.136930, Period(5, Years), Period(4, Years) },
		{ 0.126833, Period(5, Years), Period(5, Years) },
		{ 0.175963, Period(6, Years), Period(1, Years) },
		{ 0.166844, Period(6, Years), Period(2, Years) },
		{ 0.158306, Period(6, Years), Period(3, Years) },
		{ 0.136930, Period(6, Years), Period(4, Years) },
		{ 0.175963, Period(7, Years), Period(1, Years) },
		{ 0.166844, Period(7, Years), Period(2, Years) },
		{ 0.158306, Period(7, Years), Period(3, Years) },
		{ 0.175963, Period(8, Years), Period(1, Years) },
		{ 0.166844, Period(8, Years), Period(2, Years) },
		{ 0.166844, Period(9, Years), Period(1, Years) }

	};

	boost::shared_ptr<IborIndex> index = makeIndex();
	boost::shared_ptr<LiborForwardModelProcess> process(
		new LiborForwardModelProcess(size, index));
	Handle<YieldTermStructure> termStructure = index->forwardingTermStructure();

	// set-up the model
	boost::shared_ptr<LmVolatilityModel> volaModel(
		new LmExtLinearExponentialVolModel(process->fixingTimes(),
		0.5, 0.6, 0.1, 0.1));

	boost::shared_ptr<LmCorrelationModel> corrModel(
		new LmLinearExponentialCorrelationModel(size, 0.5, 0.8));

	boost::shared_ptr<LiborForwardModel> model(
		new LiborForwardModel(process, volaModel, corrModel));

	Size swapVolIndex = 0;
	DayCounter dayCounter = index->forwardingTermStructure()->dayCounter();

	// set-up calibration helper
	std::vector<boost::shared_ptr<CalibrationHelper> > calibrationHelper;

	Size i;

	for (i = 0; i < swaptions.size(); i++) {
	
		Handle<Quote> swaptionVol(
			boost::shared_ptr<Quote>(
			new SimpleQuote(swaptions[i].volatility_)));

		boost::shared_ptr<CalibrationHelper> swaptionHelper(
			new SwaptionHelper(swaptions[i].maturity_, 
				swaptions[i].lenght_, swaptionVol, index,
				index->tenor(), dayCounter,
				index->dayCounter(),
				termStructure,
				CalibrationHelper::ImpliedVolError));

		swaptionHelper->setPricingEngine(
			boost::shared_ptr<PricingEngine>(
			new LfmSwaptionEngine(model, termStructure)));

		calibrationHelper.push_back(swaptionHelper);

	}

#ifdef _DEBUG

	LevenbergMarquardt om(1e-5, 1e-5, 1e-5);
	model->calibrate(calibrationHelper, om, EndCriteria(100, 20, 1e-5, 1e-5, 1e-6));

#else

	LevenbergMarquardt om(1e-6, 1e-6, 1e-6);
	model->calibrate(calibrationHelper, om, EndCriteria(500, 100, 1e-6, 1e-6, 1e-6));

#endif

	// measure the calibration error
	Real calculated = 0.0;
	for (i = 0; i<calibrationHelper.size(); ++i) {
		Real diff = calibrationHelper[i]->calibrationError();
		calculated += diff * diff;
	}

	if (std::sqrt(calculated) > tolerance)
		std::cout << "Failed to calibrate libor forward model"
		<< "\n    calculated diff: " << std::sqrt(calculated)
		<< "\n    expected : smaller than  " << tolerance << std::endl;

	// create diagnostic file
	{
		
		// build file path
		std::string fileStr("C:/Temp/liborModel_");
		fileStr.append(boost::posix_time::to_iso_string(
			boost::posix_time::second_clock::local_time()));
		fileStr.append(".csv");

		// csv builder
		utilities::csvBuilder file(fileStr);

		// optimization results
		file.add(std::string("calibration result:"), 1, 1);	// calibration result
		
		switch (model->endCriteria()) {						// different case, could use a factory...

		case EndCriteria::StationaryPoint:
			file.add(std::string("stationnary point"), 1, 2);
			break;

		case EndCriteria::MaxIterations:
			file.add(std::string("Max Iterations reached"), 1, 2);
			break;

		case EndCriteria::StationaryFunctionValue:
			file.add("Stationary Function Value reached", 1, 2);
			break;

		default:
			file.add(std::string("unknown result"), 1, 2);
		}

		// swaption volatility matrix
		file.add("correlation matrix at time zero", 2, 1);
		file.add(corrModel->correlation(0), 2, 2);
		
	}

	system("pause");
	return 0;

}
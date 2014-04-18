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
	Period     maturity_;
	Period     lenght_;

};

int main() {

	try
	{

		std::cout << "Testing calibration of a Libor forward model..."
				  << std::endl;

		/* basic settings */
		SavedSettings backup;

		const Size size_          = 20    ;					// 20 semestrers
		const Real tolerance_     = 8e-3  ;					// tolerance
		const Real convexity_     = 0.008 ;					// convexity adjustment for future prices
		const Real meanReverting_ = 0.030 ;					// mean reversion for future prices

		const Calendar calendar								// pricing calendar
			= JointCalendar(
			UnitedStates(UnitedStates::Settlement),
			UnitedKingdom(UnitedKingdom::Settlement));

		const Date pricingDate = Date(16, April, 2014);		// pricing date

		Settings::instance().evaluationDate()				// set global evaluation date
			= pricingDate;

		/* create the yield term structure */
		boost::shared_ptr<RelinkableHandle<YieldTermStructure> > ts(
			new RelinkableHandle<YieldTermStructure>());

		boost::shared_ptr<IborIndex> libor(					// libor index associated
			new USDLibor(
			Period(3, Months),
			*ts));

		libor->addFixing(Date(16, April, 2014), .0022785);	// add a few fixings
		libor->addFixing(Date(15, April, 2014), .0022635);
		libor->addFixing(Date(14, April, 2014), .0022865);
		libor->addFixing(Date(11, April, 2014), .0022645);

		// the rate helpers
		std::vector<boost::shared_ptr<RateHelper> > rateHelpers;

		std::vector<depositData> depositRates				// deposit data
			= std::vector<depositData>
		{

			{ .0009070, 0, Period(1, Days) },
			{ .0014250, 1, Period(2, Days) },
			{ .0012150, 2, Period(1, Weeks) },
			{ .0015200, 2, Period(1, Months) },
			{ .0019300, 2, Period(2, Months) }/*,
			{ .0022785, 2, Period(3, Months) }*/

		};

		std::vector<futureData> futureRates					// future data
			= std::vector<futureData>
		{

			//{ 99.7725, "EDK4" },
			{ 99.7700, "EDM4" },
			//{ 99.7650, "EDN4" },
			//{ 99.7600, "EDQ4" },
			{ 99.7600, "EDU4" },
			//{ 99.7450, "EDV4" },
			{ 99.7250, "EDZ4" },
			{ 99.6300, "EDH5" },
			{ 99.4550, "EDM5" },
			{ 99.2250, "EDU5" },
			{ 98.9450, "EDZ5" },
			{ 98.6450, "EDH6" },
			{ 98.3400, "EDM6" },
			{ 98.0500, "EDU6" },
			{ 97.7850, "EDZ6" },
			{ 97.5600, "EDU7" }

		};

		std::vector<swapData> swapRates					// future data
			= std::vector<swapData>
		{

			{ .0139980, Period(4 , Years) },
			{ .0176100, Period(5 , Years) },
			{ .0205300, Period(6 , Years) },
			{ .0228500, Period(7 , Years) },
			{ .0247290, Period(8 , Years) },
			{ .0262750, Period(9 , Years) },
			{ .0275070, Period(10, Years) },
			{ .0286800, Period(11, Years) },
			{ .0286800, Period(12, Years) }

		};

		for (std::vector<depositData>::const_iterator It = depositRates.cbegin();
			It != depositRates.cend(); It++)
		{

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
				boost::shared_ptr<RateHelper>(
					new	depositBootstrapHelper(
						quoteHandler,
						myDepositPtr)));

		}

		for (std::vector<futureData>::const_iterator It = futureRates.cbegin();
			It != futureRates.cend(); It++)
		{

			Handle<Quote> convexityHandle(					// convexity adjustment
				boost::shared_ptr<Quote>(
					new SimpleQuote(convexity_)));

			Handle<Quote> meanRevertingHandle(				// mean reverting
				boost::shared_ptr<Quote>(
					new SimpleQuote(meanReverting_)));

			Handle<Quote> futurePriceHandle(				// price
				boost::shared_ptr<Quote>(
					new SimpleQuote(It->price_)));

			Date futureDate = IMM::date(					// IMM date
				It->futureCode_.substr(2, 2),
				pricingDate);

			Handle<Quote> convexityAdjustedQuoteHendle(		// convexity adjustement
				boost::shared_ptr<Quote>(
					new futuresConvexityAdjustmentQuote2(
						libor,
						futureDate,
						futurePriceHandle,
						convexityHandle,
						meanRevertingHandle,
						pricingDate)));

			rateHelpers.push_back(							// helper
				boost::shared_ptr<FuturesRateHelper>(
					new FuturesRateHelper(
						futurePriceHandle,
						futureDate,
						libor,
						convexityAdjustedQuoteHendle)));

		}

		boost::shared_ptr<swapCurve> curve(					// building the curve object
			new swapCurve(rateHelpers, calendar, 10e-12));

		ts->linkTo(curve);									// finally relink the index

		// rought check on yield curve points
		std::cout << "yield curve value on 1W: "
			<< curve->zeroRate(1.0 / 52, Continuous)
			<< std::endl;

		std::cout << "yield curve value on 1M: "
			<< curve->zeroRate(1.0 / 12, Continuous)
			<< std::endl;

		std::cout << "yield curve value on 2M: "
			<< curve->zeroRate(2.0 / 12, Continuous)
			<< std::endl;

		std::cout << "yield curve value on 3M: "
			<< curve->zeroRate(3.0 / 12, Continuous)
			<< std::endl;

		std::cout << "yield curve value on 4M: "
			<< curve->zeroRate(4.0 / 12, Continuous)
			<< std::endl;

		std::cout << "yield curve value on 5M: "
			<< curve->zeroRate(5.0 / 12, Continuous)
			<< std::endl;

		std::cout << "yield curve value on 6M: "
			<< curve->zeroRate(.5, Continuous)
			<< std::endl;

		std::cout << "yield curve value on 1Y: "
			<< curve->zeroRate(1.0, Continuous)
			<< std::endl;

		std::cout << "yield curve value on 2Y: "
			<< curve->zeroRate(2.0, Continuous)
			<< std::endl;

		std::cout << "yield curve value on 3Y: "
			<< curve->zeroRate(3.0, Continuous)
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


		Handle<YieldTermStructure> termStructure = libor->forwardingTermStructure();

		// set up the process
		boost::shared_ptr<LiborForwardModelProcess> process(
			new LiborForwardModelProcess(size_, libor));

		// set-up the model
		boost::shared_ptr<LmVolatilityModel> volaModel(
			new LmExtLinearExponentialVolModel(process->fixingTimes(),
			0.5, 0.6, 0.1, 0.1));

		boost::shared_ptr<LmCorrelationModel> corrModel(
			new LmLinearExponentialCorrelationModel(size_, 0.5, 0.8));

		boost::shared_ptr<LiborForwardModel> model(
			new LiborForwardModel(process, volaModel, corrModel));

		Size swapVolIndex = 0;
		DayCounter dayCounter = libor->forwardingTermStructure()->dayCounter();

		// set-up calibration helper
		std::vector<boost::shared_ptr<CalibrationHelper> > calibrationHelper;

		Size i;

		for (i = 0; i < swaptions.size(); i++) {

			Handle<Quote> swaptionVol(
				boost::shared_ptr<Quote>(
				new SimpleQuote(swaptions[i].volatility_)));

			boost::shared_ptr<CalibrationHelper> swaptionHelper(
				new SwaptionHelper(swaptions[i].maturity_,
				swaptions[i].lenght_, swaptionVol, libor,
				libor->tenor(), dayCounter,
				libor->dayCounter(),
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

		if (std::sqrt(calculated) > tolerance_)
			std::cout << "Failed to calibrate libor forward model"
			<< "\n    calculated diff: " << std::sqrt(calculated)
			<< "\n    expected : smaller than  " << tolerance_ << std::endl;

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
	catch (std::exception & e){
	
		std::cout << "an error occured: "
				  << std::endl;

		std::cout << e.what() 
				  << std::endl;

		system("pause");
		return 1;
	
	}
	catch (...){

		std::cout << "an unknown error occured..."
			<< std::endl;

		system("pause");
		return 1;

	}
}
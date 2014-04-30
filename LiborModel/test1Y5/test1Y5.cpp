/*
*
* The goal of this project is to calibrate blabla...
*
*
*
*
*
*/

#include "test1Y5/test1Y5.hpp"

void test1Y5() {

	std::cout << "Testing calibration of a Libor forward model with 1Y5 settings..."
		<< std::endl;


	/* basic settings */
	SavedSettings backup;

	const Size size_ = 18;								// 24 trimesters
	const Real tolerance_ = 8e-3;						// tolerance

	boost::shared_ptr<IborIndex> libor = curveCreation();

	std::vector<swaptionData> swaptions					// swaption data
		= std::vector<swaptionData> {

			//{ 50.200, Period(1, Months), Period(1, Years) },
			//{ 54.475, Period(3, Months), Period(1, Years) },
			//{ 63.350, Period(6, Months), Period(1, Years) },
			//{ 68.650, Period(1, Years), Period(1, Years) },
			//{ 49.850, Period(2, Years), Period(1, Years) },
			//{ 38.500, Period(3, Years), Period(1, Years) },
			//{ 31.900, Period(4, Years), Period(1, Years) },
			//{ 28.500, Period(5, Years), Period(1, Years) },
			//{ 60.950, Period(1, Months), Period(2, Years) },
			{ 55.700, Period(3, Months), Period(2, Years) },
			{ 58.100, Period(6, Months), Period(2, Years) },
			{ 56.550, Period(1, Years), Period(2, Years) },
			//{ 42.600, Period(2, Years), Period(2, Years) },
			//{ 34.300, Period(3, Years), Period(2, Years) },
			//{ 29.400, Period(4, Years), Period(2, Years) },
			{ 51.050, Period(1, Months), Period(3, Years) },
			{ 48.300, Period(3, Months), Period(3, Years) },
			{ 48.900, Period(6, Months), Period(3, Years) },
			{ 47.000, Period(1, Years), Period(3, Years) },
			//{ 37.000, Period(2, Years), Period(3, Years) },
			//{ 31.275, Period(3, Years), Period(3, Years) },
			{ 40.800, Period(1, Months), Period(4, Years) },
			{ 40.225, Period(3, Months), Period(4, Years) },
			{ 40.200, Period(6, Months), Period(4, Years) },
			{ 39.300, Period(1, Years), Period(4, Years) },
			//{ 32.800, Period(2, Years), Period(4, Years) },
			{ 36.025, Period(1, Months), Period(5, Years) },
			{ 35.600, Period(3, Months), Period(5, Years) },
			{ 35.425, Period(6, Months), Period(5, Years) },
			{ 34.250, Period(1, Years), Period(5, Years) },
			{ 30.800, Period(1, Months), Period(6, Years) },
			{ 30.700, Period(3, Months), Period(6, Years) },
			//{ 31.200, Period(6, Months), Period(6, Years) },
			{ 30.750, Period(1, Years), Period(6, Years) }
	};

	Handle<YieldTermStructure> termStructure = libor->forwardingTermStructure();

	// set up the process
	boost::shared_ptr<LiborForwardModelProcess> process(
		new LiborForwardModelProcess(size_, libor));

	std::vector<Time> fixingT = process->fixingTimes();

	// set-up the volatility model
	boost::shared_ptr<LmVolatilityModel> volaModel(
		new LmLinearExponentialVolatilityModel(process->fixingTimes(),
		0.5, 0.5, 0.5, 0.5));

	//boost::shared_ptr<LmCorrelationModel> underlyingCorrModel(
	//	new LmLinearExponentialCorrelationModel(size_, 0.7, 0.2, 3));
	
	boost::shared_ptr<LmCorrelationModel> underlyingCorrModel( // todo: calibrate on real data
		new LmExponentialCorrelationModel(size_, 0.01));

	boost::shared_ptr<LmCorrelationModel> corrModel(
		new LmConstWrapperCorrelationModel(underlyingCorrModel));

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
			new SimpleQuote(swaptions[i].volatility_ / 100)));

		boost::shared_ptr<CalibrationHelper> swaptionHelper(
			new SwaptionHelper(swaptions[i].lenght_,
			swaptions[i].maturity_, swaptionVol, libor,
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

	boost::shared_ptr<OptimizationMethod> om(
		new LevenbergMarquardt(1e-12, 1e-12, 1e-12));

	model->calibrate(calibrationHelper, *om, EndCriteria(5000, 50, 1e-12, 1e-12, 1e-12));

#endif
	// measure the calibration error
	Array diff(calibrationHelper.size(), 0.0);
	Real ssr = 0.0;
	for (i = 0; i < calibrationHelper.size(); ++i) {

		diff[i] = calibrationHelper[i]->calibrationError();
		ssr = ssr + diff[i] * diff[i];
	}

	// create diagnostic file
	{

		std::string fileStr("C:/Temp/liborModel_1Y5_");			// build file path
		fileStr.append(boost::posix_time::to_iso_string(
			boost::posix_time::second_clock::local_time()));
		fileStr.append(".csv");

		utilities::csvBuilder file(fileStr);				// csv builder

		Array times(size_, 0.0); Array rates(size_, 0.0);	// saves yield curve data

		for (int i = 0; i < size_; i++) {

			times[i] = fixingT[i];							// the fixing times from model
			rates[i] = libor->forwardingTermStructure()->zeroRate(
				times[i], Continuous);

		}

		file.add("times", 1, 1); file.add("rates", 1, 2);	// adds the yield curve data
		file.add(times, 2, 1); file.add(rates, 2, 2);

		file.add(std::string("calibration result:"), 1, 4);	// calibration result
		file.add(model->endCriteria(), 2, 4);

		file.add(std::string("calculated diff:"), 4, 4);	// calibration result
		file.add(std::sqrt(ssr), 5, 4);
		
		file.add(std::string("individual errors:"), 7, 4);	// individual errors
		file.add(diff, 8, 4);

		file.add("volatility array at time zero", 1, 6);	// volatiltity in t=0
		file.add(volaModel->volatility(0), 2, 6);

		file.add("correlation matrix at time zero", 1, 8);	// correlation in t=0
		file.add(corrModel->correlation(0), 2, 8);		
		
		file.add("parameters", 30, 1);						// calibrated parameters
		file.add(model->params(), 31, 1);
	}

}
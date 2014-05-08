/*
*
* The goal of this project is to calibrate blabla...
*
*
*
*
*
*/

#include "test1Y10/test1Y10.hpp"

void test1Y10(boost::shared_ptr<IborIndex> libor,
			  utilities::csvBuilder & file) {

	std::cout << "Testing calibration of a Libor forward model with 1Y10 settings..."
		<< std::endl;


	/* basic settings */
	SavedSettings backup;

	const Size size_ = 44;								// 44 trimesters
	const Real tolerance_ = 8e-3;						// tolerance

	boost::shared_ptr<IborIndex> libor = curveCreation();

	std::vector<swaptionData> swaptions					// swaption data
		= std::vector<swaptionData> {

			//{ 50.200, Period(1, Months), Period(1, Years) },
			{ 54.475, Period(3, Months), Period(1, Years) },
			{ 63.350, Period(6, Months), Period(1, Years) },
			//{ 68.650, Period(1, Years), Period(1, Years) },
			//{ 49.850, Period(2, Years), Period(1, Years) },
			//{ 38.500, Period(3, Years), Period(1, Years) },
			//{ 31.900, Period(4, Years), Period(1, Years) },
			//{ 28.500, Period(5, Years), Period(1, Years) },
			{ 26.500, Period(6, Years), Period(1, Years) },
			{ 24.625, Period(7, Years), Period(1, Years) },
			{ 23.500, Period(8, Years), Period(1, Years) },
			{ 22.550, Period(9, Years), Period(1, Years) },
			{ 21.150, Period(10, Years), Period(1, Years) },
			//{ 60.950, Period(1, Months), Period(2, Years) },
			{ 55.700, Period(3, Months), Period(2, Years) },
			{ 58.100, Period(6, Months), Period(2, Years) },
			{ 56.550, Period(1, Years), Period(2, Years) },
			//{ 42.600, Period(2, Years), Period(2, Years) },
			//{ 34.300, Period(3, Years), Period(2, Years) },
			//{ 29.400, Period(4, Years), Period(2, Years) },
			{ 26.900, Period(5, Years), Period(2, Years) },
			{ 25.050, Period(6, Years), Period(2, Years) },
			{ 23.800, Period(7, Years), Period(2, Years) },
			{ 22.700, Period(8, Years), Period(2, Years) },
			{ 21.600, Period(9, Years), Period(2, Years) },
			{ 51.050, Period(1, Months), Period(3, Years) },
			{ 48.300, Period(3, Months), Period(3, Years) },
			{ 48.900, Period(6, Months), Period(3, Years) },
			{ 47.000, Period(1, Years), Period(3, Years) },
			//{ 37.000, Period(2, Years), Period(3, Years) },
			//{ 31.275, Period(3, Years), Period(3, Years) },
			{ 27.650, Period(4, Years), Period(3, Years) },
			{ 25.650, Period(5, Years), Period(3, Years) },
			{ 24.200, Period(6, Years), Period(3, Years) },
			{ 23.050, Period(7, Years), Period(3, Years) },
			{ 22.050, Period(8, Years), Period(3, Years) },
			{ 40.800, Period(1, Months), Period(4, Years) },
			{ 40.225, Period(3, Months), Period(4, Years) },
			{ 40.200, Period(6, Months), Period(4, Years) },
			{ 39.300, Period(1, Years), Period(4, Years) },
			//{ 32.800, Period(2, Years), Period(4, Years) },
			{ 28.925, Period(3, Years), Period(4, Years) },
			{ 26.150, Period(4, Years), Period(4, Years) },
			{ 24.650, Period(5, Years), Period(4, Years) },
			{ 23.500, Period(6, Years), Period(4, Years) },
			{ 22.350, Period(7, Years), Period(4, Years) },
			{ 36.025, Period(1, Months), Period(5, Years) },
			{ 35.600, Period(3, Months), Period(5, Years) },
			{ 35.425, Period(6, Months), Period(5, Years) },
			{ 34.250, Period(1, Years), Period(5, Years) },
			{ 29.850, Period(2, Years), Period(5, Years) },
			{ 26.900, Period(3, Years), Period(5, Years) },
			{ 24.925, Period(4, Years), Period(5, Years) },
			{ 23.700, Period(5, Years), Period(5, Years) },
			{ 22.750, Period(6, Years), Period(5, Years) },
			{ 30.800, Period(1, Months), Period(6, Years) },
			{ 30.700, Period(3, Months), Period(6, Years) },
			//{ 31.200, Period(6, Months), Period(6, Years) },
			{ 30.750, Period(1, Years), Period(6, Years) },
			{ 27.850, Period(2, Years), Period(6, Years) },
			{ 25.775, Period(3, Years), Period(6, Years) },
			{ 24.125, Period(4, Years), Period(6, Years) },
			{ 23.100, Period(5, Years), Period(6, Years) },
			{ 27.600, Period(1, Months), Period(7, Years) },
			{ 27.750, Period(3, Months), Period(7, Years) },
			{ 28.350, Period(6, Months), Period(7, Years) },
			{ 28.200, Period(1, Years), Period(7, Years) },
			{ 26.400, Period(2, Years), Period(7, Years) },
			{ 24.750, Period(3, Years), Period(7, Years) },
			{ 23.400, Period(4, Years), Period(7, Years) },
			{ 25.250, Period(1, Months), Period(8, Years) },
			{ 25.450, Period(3, Months), Period(8, Years) },
			{ 26.000, Period(6, Months), Period(8, Years) },
			{ 26.250, Period(1, Years), Period(8, Years) },
			{ 25.225, Period(2, Years), Period(8, Years) },
			{ 23.875, Period(3, Years), Period(8, Years) },
			{ 23.575, Period(1, Months), Period(9, Years) },
			{ 23.575, Period(3, Months), Period(9, Years) },
			{ 24.600, Period(6, Months), Period(9, Years) },
			{ 24.800, Period(1, Years), Period(9, Years) },
			{ 24.150, Period(2, Years), Period(9, Years) },
			{ 22.275, Period(1, Months), Period(10, Years) },
			{ 22.125, Period(3, Months), Period(10, Years) },
			{ 23.300, Period(6, Months), Period(10, Years) },
			{ 23.600, Period(1, Years), Period(10, Years) }

	};

	Handle<YieldTermStructure> termStructure = libor->forwardingTermStructure();

	// set up the process
	boost::shared_ptr<LiborForwardModelProcess> process(
		new LiborForwardModelProcess(size_, libor));

	std::vector<Time> fixingT = process->fixingTimes();

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
		new LevenbergMarquardt(1e-6, 1e-6, 1e-6));
	//boost::shared_ptr<OptimizationMethod> om(new SteepestDescent);
	model->calibrate(calibrationHelper, *om, EndCriteria(2000, 100, 1e-6, 1e-6, 1e-6));

#endif
	// measure the calibration error
	Real calculated = 0.0;
	for (i = 0; i<calibrationHelper.size(); ++i) {
		Real diff = calibrationHelper[i]->calibrationError();
		calculated += diff * diff;
	}

	// create diagnostic file
	{

		std::string fileStr("C:/Temp/liborModel_1Y10_");			// build file path
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
		file.add(std::sqrt(calculated), 5, 4);

		file.add("correlation matrix at time zero", 1, 6);	// correlation
		file.add(corrModel->correlation(0), 2, 6);

	}

}
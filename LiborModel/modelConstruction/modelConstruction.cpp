/*
*
* The goal of this project is to calibrate blabla...
*
*
*
*
*
*/

#include "modelConstruction/modelConstruction.hpp"

boost::shared_ptr<LiborForwardModel> & modelConstruction(
	const boost::shared_ptr<IborIndex> & libor,
	const Real & max,
	utilities::csvBuilder & file,
	bool force) {

	std::cout << "Calibration of a "
			  << "Libor forward model"
			  << std::endl;

	double a = .35;										//custom a
	if (force == true) {
	
		std::cout << "please enter correlation parameter 1:"
			<< std::endl;
		std::cin >> a;
	
	}
	
	double b = 0.65;									//custom b
	if (force == true) {

		std::cout << "please enter correlation parameter 1:"
			<< std::endl;
		std::cin >> b;

	}

	SavedSettings backup;								// basic settings
	const Size size_ = 18;								// 24 trimesters
	const Real tolerance_ = 8e-3;						// tolerance
	Date pricingDate =									// pricing date
		Settings::instance().evaluationDate();

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

	Handle<YieldTermStructure> termStructure =				// ts handle
		libor->forwardingTermStructure();

	// set up the process
	boost::shared_ptr<LiborForwardModelProcess> process(
		new LiborForwardModelProcess(size_, libor));

	std::vector<Time> fixingT = process->fixingTimes();

	// set-up the volatility model -> close to expected values to speed-yup calibration
	boost::shared_ptr<LmVolatilityModel> volaModel(
		new LmLinearExponentialVolatilityModel(process->fixingTimes(),
		0.9, 1.2, 0.35, 0.1));

	boost::shared_ptr<LmCorrelationModel> underlyingCorrModel(
		new LmLinearExponentialCorrelationModel(size_, a, b, 3));

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

		if (swaptions[i].lenght_ + swaptions[i].maturity_ <= max) {

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
	}

	Size maxIterations;
	Size maxStationaryStateIteration;

#ifdef _DEBUG

	maxIterations = 100;
	maxStationaryStateIteration = 20;

#else

	maxIterations = 10000;
	maxStationaryStateIteration = 100;

#endif

	boost::shared_ptr<OptimizationMethod> om(
		new LevenbergMarquardt(1e-12, 1e-12, 1e-12));

	model->calibrate(
		calibrationHelper, *om,
		EndCriteria(
		maxIterations, 
		maxStationaryStateIteration,
		1e-5, 1e-5, 1e-6));

	// measure the calibration error
	Array diff(calibrationHelper.size(), 0.0);
	Real ssr = 0.0;
	for (i = 0; i < calibrationHelper.size(); ++i) {

		diff[i] = calibrationHelper[i]->calibrationError();
		ssr = ssr + diff[i] * diff[i];
	}

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
		
	file.add(std::string("individual errors:"), 21, 1);	// individual errors
	file.add(diff, 22, 1);

	file.add("volatility array at time zero", 1, 6);	// volatiltity in t=0
	file.add(volaModel->volatility(0), 2, 6);

	file.add("correlation matrix at time zero", 1, 8);	// correlation in t=0
	file.add(corrModel->correlation(0), 2, 8);		
		
	file.add("parameters", 7, 4);						// calibrated parameters
	file.add(model->params(), 8, 4);

	file.add("correlation poarameters", 13, 4);			// parameters used for correlation
	file.add(a, 14, 4);
	file.add(b, 15, 4);

	// step 2: simulation
	const Size steps = 1000;								// number of steps
	const Size nrTrails = 5000;								// number of paths

	std::cout << "Calibration completed, "					// information message
			  << "starting simulation phase." 
			  << std::endl
			  << "Generating "
			  << boost::lexical_cast<std::string, Size>(nrTrails)
			  << " with "
			  << boost::lexical_cast<std::string, Size>(steps)
			  << " for each path..." 
			  << std::endl;
		
	boost::shared_ptr<LiborForwardModelProcess> fwdProcess(	// create forward process
		new LiborForwardModelProcess(size_, libor));

	fwdProcess->setCovarParam(								// set-up pricing engine
		boost::shared_ptr<LfmCovarianceParameterization>(
			new LfmCovarianceProxy(volaModel, corrModel)));

	typedef PseudoRandom::rsg_type rsg_type;				// RNG definitions
	typedef MultiPathGenerator<rsg_type>::sample_type sample_type;

	std::vector<Time> tmp = fwdProcess->fixingTimes();
	TimeGrid grid(tmp.begin(), tmp.end(), steps);			// creates time grid

	std::vector<Size> location;
	for (Size i = 0; i < tmp.size(); ++i) {
		location.push_back(
			std::find(grid.begin(), grid.end(), tmp[i]) - grid.begin());
	}

	BigNatural seed;

#ifdef _DEBUG
	seed = 42;
#else
	seed = SeedGenerator::instance().get();									// seed
#endif
	
	rsg_type rsg = PseudoRandom::make_sequence_generator(
		fwdProcess->factors()*(grid.size() - 1),
		seed);

	MultiPathGenerator<rsg_type> generator(fwdProcess, grid, rsg, false);

	// produce 1 simulation
	Sample<MultiPath> s = generator.next();
	
	// copy in a matrix
	Matrix values(s.value.pathSize(), s.value.assetNumber(), 0.0);

	for (Size i = 0; i < s.value.pathSize(); i++) {
	
		for (Size j = 0; j < s.value.assetNumber(); j++)
			values[i][j] = s.value[j][i];
	
	}

	file.add("sample path generated:", 22, 3);
	file.add(values, 22, 3);

	return model;
	
}
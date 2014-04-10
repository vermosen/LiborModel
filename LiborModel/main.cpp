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

struct dataSet {										// static struct

	/* time data */
	Date referenceDate_;
	Date endDate_;
	Calendar calendar_;

	/* termstructure data */
	boost::shared_ptr<YieldTermStructure> ts_;

	/* initial rates */
	std::vector<Rate> initialRates_;

	/* daycounter */
	DayCounter dayCounter_;

	/* rate maturities (temp) */
	std::vector<Time> maturities_;

	/* time differences, close to time fractions */
	std::vector<Real> accruals_;
	
};

static dataSet tt;												// global variable declaration


void setup() {

	// Times
	tt.calendar_ = NullCalendar();
	Settings::instance().evaluationDate = Date(7, April, 2014);
	tt.referenceDate_ = Settings::instance().evaluationDate();
	tt.endDate_ = tt.referenceDate_ + 5 * Years;

	// termstructure Initialization
	tt.dayCounter_ = Actual365Fixed();

	tt.ts_ = boost::shared_ptr<YieldTermStructure>(
		new FlatForward(
		tt.referenceDate_,
		0.05, tt.dayCounter_,
		Continuous, Annual));

	// create the schredule for the dates
	Schedule dates(
		tt.referenceDate_, 
		tt.endDate_, 
		Period(Semiannual),
		tt.calendar_, Following, 
		Following, DateGeneration::Backward, 
		false);

	tt.maturities_ = std::vector<Time>(dates.size() - 1);
	tt.accruals_ = std::vector<Real>(tt.maturities_.size() - 1);
	
	for (Size i = 1; i<dates.size(); ++i)
		tt.maturities_[i - 1] = tt.dayCounter_.yearFraction(tt.referenceDate_, dates[i]);
	
	for (Size i = 1; i<tt.maturities_.size(); ++i)
		tt.accruals_[i - 1] = tt.maturities_[i] - tt.maturities_[i - 1];

	// Rates & displacement
	todaysForwards_ = std::vector<Rate>(accruals_.size());
	numberOfFactors_ = 3;
	alpha_ = -0.05;
	alphaMax_ = 1.0;
	alphaMin_ = -1.0;
	displacement_ = 0.0;
	for (Size i = 0; i<todaysForwards_.size(); ++i) {
		todaysForwards_[i] = 0.03 + 0.0025*i;
		//todaysForwards_[i] = 0.03;
	}
	LMMCurveState curveState_lmm(rateTimes_);
	curveState_lmm.setOnForwardRates(todaysForwards_);
	todaysSwaps_ = curveState_lmm.coterminalSwapRates();

	// Discounts
	todaysDiscounts_ = std::vector<DiscountFactor>(rateTimes_.size());
	todaysDiscounts_[0] = 0.95;
	for (Size i = 1; i<rateTimes_.size(); ++i)
		todaysDiscounts_[i] = todaysDiscounts_[i - 1] /
		(1.0 + todaysForwards_[i - 1] * accruals_[i - 1]);

	//// Swaption Volatilities
	//Volatility mktSwaptionVols[] = {
	//                        0.15541283,
	//                        0.18719678,
	//                        0.20890740,
	//                        0.22318179,
	//                        0.23212717,
	//                        0.23731450,
	//                        0.23988649,
	//                        0.24066384,
	//                        0.24023111,
	//                        0.23900189,
	//                        0.23726699,
	//                        0.23522952,
	//                        0.23303022,
	//                        0.23076564,
	//                        0.22850101,
	//                        0.22627951,
	//                        0.22412881,
	//                        0.22206569,
	//                        0.22009939
	//};

	//a = -0.0597;
	//b =  0.1677;
	//c =  0.5403;
	//d =  0.1710;

	a_ = 0.0;
	b_ = 0.17;
	c_ = 1.0;
	d_ = 0.10;

	Volatility mktCapletVols[] = {
		0.1640,
		0.1740,
		0.1840,
		0.1940,
		0.1840,
		0.1740,
		0.1640,
		0.1540,
		0.1440,
		0.1340376439125532
	};

	//swaptionDisplacedVols = std::vector<Volatility>(todaysSwaps.size());
	//swaptionVols = std::vector<Volatility>(todaysSwaps.size());
	//capletDisplacedVols = std::vector<Volatility>(todaysSwaps.size());
	capletVols_.resize(todaysSwaps_.size());
	for (Size i = 0; i<todaysSwaps_.size(); i++) {
		//    swaptionDisplacedVols[i] = todaysSwaps[i]*mktSwaptionVols[i]/
		//                              (todaysSwaps[i]+displacement);
		//    swaptionVols[i]= mktSwaptionVols[i];
		//    capletDisplacedVols[i] = todaysForwards[i]*mktCapletVols[i]/
		//                            (todaysForwards[i]+displacement);
		capletVols_[i] = mktCapletVols[i];
	}

	// Cap/Floor Correlation
	longTermCorrelation_ = 0.5;
	beta_ = 0.2;
	measureOffset_ = 5;

	// Monte Carlo
	seed_ = 42;

#ifdef _DEBUG
	paths_ = 127;
	trainingPaths_ = 31;
#else
	paths_ = 32767; //262144-1; //; // 2^15-1
	trainingPaths_ = 8191; // 2^13-1
#endif
}

void setup() {

	// Times
	calendar_ = NullCalendar();
	todaysDate_ = Settings::instance().evaluationDate();
	//startDate = todaysDate + 5*Years;
	endDate_ = todaysDate_ + 66 * Months;
	Schedule dates(todaysDate_, endDate_, Period(Semiannual),
		calendar_, Following, Following, DateGeneration::Backward, false);
	rateTimes_ = std::vector<Time>(dates.size() - 1);
	accruals_ = std::vector<Real>(rateTimes_.size() - 1);
	dayCounter_ = SimpleDayCounter();
	for (Size i = 1; i<dates.size(); ++i)
		rateTimes_[i - 1] = dayCounter_.yearFraction(todaysDate_, dates[i]);
	for (Size i = 1; i<rateTimes_.size(); ++i)
		accruals_[i - 1] = rateTimes_[i] - rateTimes_[i - 1];

	// Rates & displacement
	todaysForwards_ = std::vector<Rate>(accruals_.size());
	numberOfFactors_ = 3;
	alpha_ = -0.05;
	alphaMax_ = 1.0;
	alphaMin_ = -1.0;
	displacement_ = 0.0;
	for (Size i = 0; i<todaysForwards_.size(); ++i) {
		todaysForwards_[i] = 0.03 + 0.0025*i;
		//todaysForwards_[i] = 0.03;
	}
	LMMCurveState curveState_lmm(rateTimes_);
	curveState_lmm.setOnForwardRates(todaysForwards_);
	todaysSwaps_ = curveState_lmm.coterminalSwapRates();

	// Discounts
	todaysDiscounts_ = std::vector<DiscountFactor>(rateTimes_.size());
	todaysDiscounts_[0] = 0.95;
	for (Size i = 1; i<rateTimes_.size(); ++i)
		todaysDiscounts_[i] = todaysDiscounts_[i - 1] /
		(1.0 + todaysForwards_[i - 1] * accruals_[i - 1]);

	//// Swaption Volatilities
	//Volatility mktSwaptionVols[] = {
	//                        0.15541283,
	//                        0.18719678,
	//                        0.20890740,
	//                        0.22318179,
	//                        0.23212717,
	//                        0.23731450,
	//                        0.23988649,
	//                        0.24066384,
	//                        0.24023111,
	//                        0.23900189,
	//                        0.23726699,
	//                        0.23522952,
	//                        0.23303022,
	//                        0.23076564,
	//                        0.22850101,
	//                        0.22627951,
	//                        0.22412881,
	//                        0.22206569,
	//                        0.22009939
	//};

	//a = -0.0597;
	//b =  0.1677;
	//c =  0.5403;
	//d =  0.1710;

	a_ = 0.0;
	b_ = 0.17;
	c_ = 1.0;
	d_ = 0.10;

	Volatility mktCapletVols[] = {
		0.1640,
		0.1740,
		0.1840,
		0.1940,
		0.1840,
		0.1740,
		0.1640,
		0.1540,
		0.1440,
		0.1340376439125532
	};

	//swaptionDisplacedVols = std::vector<Volatility>(todaysSwaps.size());
	//swaptionVols = std::vector<Volatility>(todaysSwaps.size());
	//capletDisplacedVols = std::vector<Volatility>(todaysSwaps.size());
	capletVols_.resize(todaysSwaps_.size());
	for (Size i = 0; i<todaysSwaps_.size(); i++) {
		//    swaptionDisplacedVols[i] = todaysSwaps[i]*mktSwaptionVols[i]/
		//                              (todaysSwaps[i]+displacement);
		//    swaptionVols[i]= mktSwaptionVols[i];
		//    capletDisplacedVols[i] = todaysForwards[i]*mktCapletVols[i]/
		//                            (todaysForwards[i]+displacement);
		capletVols_[i] = mktCapletVols[i];
	}

	// Cap/Floor Correlation
	longTermCorrelation_ = 0.5;
	beta_ = 0.2;
	measureOffset_ = 5;

	// Monte Carlo
	seed_ = 42;

#ifdef _DEBUG
	paths_ = 127;
	trainingPaths_ = 31;
#else
	paths_ = 32767; //262144-1; //; // 2^15-1
	trainingPaths_ = 8191; // 2^13-1
#endif
}

const boost::shared_ptr<SequenceStatisticsInc> simulate(
	const boost::shared_ptr<MarketModelEvolver>& evolver,
	const MarketModelMultiProduct& product) {
	Size initialNumeraire = evolver->numeraires().front();
	Real initialNumeraireValue = todaysDiscounts_[initialNumeraire];

	AccountingEngine engine(evolver, product, initialNumeraireValue);
	boost::shared_ptr<SequenceStatisticsInc> stats(
		new SequenceStatisticsInc(product.numberOfProducts()));
	engine.multiplePathValues(*stats, paths_);
	return stats;
}

int main() {

	try {

		// set the evaluation date
		Settings::instance().evaluationDate() = tt.reference_;

		// load the yield curve
		tt.initializeTermStructure()

		// load the correlations
		Matrix corr = tt.correlations()->correlation(0);

		// creating the model
		boost::shared_ptr<MarketModel> model = tt.model();

		// create a sobol BM generator factory
		SobolBrownianGeneratorFactory generatorFactory(
			SobolBrownianGenerator::Diagonal, seed);

		// generate the numeraires
		std::vector<Size> numeraires(moneyMarketMeasure(*tt.evolution()));

		// create the evolver
		LogNormalFwdRatePc evolver(
			model,
			generatorFactory,
			numeraires);

		// creating .csv file
		QuantLib::utilities::csvBuilder test("C://Temp/test.csv");

		test.add(corr, 1, 1);									// add correlation matrix

		system("pause");										// console makes a pause

		return 0;

	}
	catch (std::exception & e) {
	
		std::cout << "an error occurred: "
			<< std::endl
			<< e.what()
			<< std::endl;

		system("pause");
		return 1;
	
	}
	catch (...) {

		std::cout << "an unknown error occurred" << std::endl;
		system("pause");
		return 1;

	}


}
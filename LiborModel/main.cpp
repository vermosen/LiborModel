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

	/* general data */
	Date reference_ = Date(7, April, 2014);

	Calendar calendar_ = QuantLib::UnitedStates(QuantLib::UnitedStates::Settlement);

	/* initial rates */
	std::vector<Time> initialRates_ = std::vector<Time>{
		.15, .18, .20, .23, .04, 1.5, 2.0};

	/* rate maturities (temp) */
	std::vector<Time> maturities_ = std::vector<Time>{
		.10, .25, .50, .75, 1.0, 1.5, 2.0, 3.0};

	/* initial volatilities */
	std::vector<Volatility> volatilities_ = std::vector<Volatility>{
		.05, .05, .05 , .045, .045, .04, .04};

	/* spreads.. Check what is the purpose here... */
	std::vector<Spread> displacements_ = std::vector<Spread>{
		.0, .0, .0, .0, .0, .0, .0};

	Size numberOfFactors_ = std::min<Size>((Size)5, maturities_.size());

	Real volLevel_ = 0.11;
	
	Real beta_ = 0.2;
	
	Real gamma_ = 1.0;
	
	/* termstructure building */
	boost::shared_ptr<YieldTermStructure> termStructure() {

		return boost::shared_ptr<YieldTermStructure>(
			new FlatForward(
				this->reference_,
				0.05, Actual365Fixed(),
				Continuous, Annual));

	}

	/* correlation structure */
	boost::shared_ptr<PiecewiseConstantCorrelation> correlations() {

		Real volLevel = 0.11;
		Real beta = 0.2;
		Real gamma = 1.0;

		return boost::shared_ptr<PiecewiseConstantCorrelation>(
			new ExponentialForwardCorrelation(
				maturities_,
				volLevel, 
				beta, gamma));

	}

	/* volatility structure initializer */
	boost::shared_ptr<MarketModel> model() {

		return boost::shared_ptr<MarketModel>(
			new FlatVol(
				volatilities_,
				correlations(),
				*evolution(),
				numberOfFactors_,
				initialRates_,
				displacements_));

	}

	// create the evolution description, i.e the map of maturities to generate
	boost::shared_ptr<EvolutionDescription> evolution() {
	
		return boost::shared_ptr<EvolutionDescription>(
			new EvolutionDescription(maturities_));
	
	}

};

static dataSet tt;												// global variable declaration

const Size seed = 12332;										// define a seed for simulation

int main() {

	try {

		// load the yield curve
		boost::shared_ptr<YieldTermStructure> ts = tt.termStructure();

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
		LogNormalFwdRatePc  evolver(
			model,
			generatorFactory,
			numeraires);

		// creating .csv file
		QuantLib::utilities::csvBuilder test("C://Temp/test.csv");

		test.add(corr, 1, 1);

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
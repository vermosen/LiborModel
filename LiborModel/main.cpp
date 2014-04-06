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

	std::vector<Time> maturities_ = std::vector<Time>{0.10, 0.25, 0.50, 0.75, 1.0, 1.5, 2.0};

	/* termstructure building */
	boost::shared_ptr<YieldTermStructure> termStructure() {

		boost::shared_ptr<YieldTermStructure> ts(
			new FlatForward(
				this->reference_,
				0.05, Actual365Fixed(),
				Continuous, Annual));

		return ts;

	}

	/* termstructure building */
	boost::shared_ptr<PiecewiseConstantCorrelation> correlations() {

		Real volLevel = 0.11;
		Real beta = 0.2;
		Real gamma = 1.0;
		Size numberOfFactors = std::min<Size>(5, maturities_.size());

		boost::shared_ptr<PiecewiseConstantCorrelation> corr(
			new ExponentialForwardCorrelation(
				maturities_,
				volLevel, 
				beta, gamma));

		return corr;

	}

};

static dataSet tt;												// global variable declaration

int main() {

	try {

		// load the yield curve
		boost::shared_ptr<YieldTermStructure> ts = tt.termStructure();

		// load the correlations
		Matrix corr = tt.correlations()->correlation(0);

		// creating the correlation matrix

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
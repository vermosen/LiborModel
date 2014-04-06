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

static struct dataSet {										// static struct

	Date reference_ = Date(7, April, 2014);
	Calendar calendar_ = QuantLib::UnitedStates(QuantLib::UnitedStates::Settlement);

	// build the termstructure
	boost::shared_ptr<YieldTermStructure> termStructure() {

		boost::shared_ptr<YieldTermStructure> ts(
			new FlatForward(this->reference_,
			0.05, Actual365Fixed(),
			Continuous, Annual));

		return ts;

	}

};

dataSet tt;													// global variable declaration


int main() {

	try {

		// load the yield curve
		boost::shared_ptr<YieldTermStructure> ts = tt.termStructure();

		// creating the correlation matrix

		// creating .csv file
		QuantLib::utilities::csvBuilder test("C://Temp/test.csv");

		test.add(10.0, 1, 1);

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
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

static struct dataSet {

	Date reference_ = Date(7, April, 2014);
	Calendar calendar_ = QuantLib::UnitedStates(QuantLib::UnitedStates::Settlement);


};

// build the termstructure
boost::shared_ptr<YieldTermStructure> termStructure(const dataSet & ds) {		

	Date reference(7, April, 2014);							// reference date
	boost::shared_ptr<YieldTermStructure> ts(
		new FlatForward(ds.reference_,
						0.05, Actual365Fixed(), 
						Continuous, Annual));

	return ts;

}

int main() {

	try {

		dataSet ds;

		// creating the yield curve
		boost::shared_ptr<YieldTermStructure> ts = termStructure(ds);

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
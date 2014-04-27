/*
*
* The goal of this project is to calibrate blabla...
*
*
*
*
*
*/

#include "testGridSearch/testGridSearch.hpp"

void testGridSearch() {

	std::cout << "test the grid search algorithm" << std::endl;

	// set the problem
	boost::shared_ptr<QuantLib::CostFunction> C(
		new squareProductFunction);

	QuantLib::Problem P(*C, QuantLib::NoConstraint());

	QuantLib::D6GridSearch search;

	search.setGrid(QuantLib::Array(6, -1.0), 
				   QuantLib::Array(6, 1.0), 
				   QuantLib::Array(6, 0.2));

	// end criteria -> 
	QuantLib::EndCriteria ed(1, 1, 1e-8, 1e-8, 1e-8);

	QuantLib::EndCriteria::Type end = search.minimize(P, ed);


	std::cout << "optimization results: " 
			  << end
			  << std::endl;

	std::cout << "solution found: "
			  << P.currentValue()
			  << std::endl;
}
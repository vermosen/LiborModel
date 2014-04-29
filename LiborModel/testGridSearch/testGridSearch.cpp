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

	QuantLib::GridSearch<QuantLib::D6MultiGrid> search;

	// grid with first dimension flat
	QuantLib::Array min(6, -1.0); min[0] = .5;
	QuantLib::Array max(6, 1.0); max[0] = .5;
	QuantLib::Array step(6, .2);

	search.setGrid(
		min, 
		max, 
		step);

	// end criteria
	QuantLib::EndCriteria ed(1, 1, 1e-8, 1e-8, 1e-8);

	QuantLib::EndCriteria::Type end = search.minimize(P, ed);

	std::cout << "optimization results: " 
			  << end
			  << std::endl;

	std::cout << "solution found: "
			  << P.currentValue()
			  << std::endl;
}
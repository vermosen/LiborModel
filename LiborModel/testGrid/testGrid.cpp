/*
*
* The goal of this project is to calibrate blabla...
*
*
*
*
*
*/

#include "testGrid/testGrid.hpp"

void testGrid() {

	std::cout << "generate a 6D grid" << std::endl;

	QuantLib::D6MultiGrid grid;

	grid.addDimensionStep(0, 0.0, 1.0, 0.2);
	grid.addDimensionStep(1, 0.0, 1.0, 0.2);
	grid.addDimensionStep(2, 0.0, 1.0, 0.2);
	grid.addDimensionStep(3, 0.0, 1.0, 0.2);
	grid.addDimensionStep(4, 0.0, 1.0, 0.2);
	grid.addDimensionStep(5, 0.0, 1.0, 0.1);

	std::vector<QuantLib::Array> res = grid.results();

	std::cout << "succesfully generated " 
			  << res.size() 
			  << " elements" 
			  << std::endl;

}
#ifndef test_grid_search
#define test_grid_search

#include <iostream>

#include <ql/math/optimization/constraint.hpp>
#include <ql/math/optimization/costfunction.hpp>

#include "GridSearch.hpp"

class productFunction : public QuantLib::CostFunction {

	QuantLib::Real value(const QuantLib::Array& x) const {
	
		QuantLib::Real result = 1;
		for (QuantLib::Size i = 0; i < x.size(); i++)
			result *= x[i];
	
		return result;

	};

	//! method to overload to compute the cost function values in x
	virtual QuantLib::Disposable<QuantLib::Array> values(
		const QuantLib::Array& x) const {
	
		QuantLib::Array y = x;
		return y;

	};

};
void testGridSearch();

#endif
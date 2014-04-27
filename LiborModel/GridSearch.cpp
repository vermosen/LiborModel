/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
Copyright (C) 2006 Klaus Spanderen

This file is part of QuantLib, a free-software/open-source library
for financial quantitative analysts and developers - http://quantlib.org/

QuantLib is free software: you can redistribute it and/or modify it
under the terms of the QuantLib license.  You should have received a
copy of the license along with this program; if not, please email
<quantlib-dev@lists.sf.net>. The license is also available online at
<http://quantlib.org/license.shtml>.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

#include "GridSearch.hpp"

namespace QuantLib {

	D6GridSearch::D6GridSearch(
		Real epsfcn,
		Real xtol,
		Real gtol) : epsfcn_(epsfcn), xtol_(xtol), gtol_(gtol) {}

	void D6GridSearch::setGrid(
		const Array & min,
		const Array & max,
		const Array & step) {
	
		QL_REQUIRE(max.size() == size_, "grid max values has wrong dimensions");
		QL_REQUIRE(max.size() == size_, "grid min values has wrong dimensions");
		QL_REQUIRE(max.size() == size_, "grid step values has wrong dimensions");

		for (Size i = 0; i < size_; i++)
			QL_REQUIRE(max[i] >= min[i], 
			"one of the maximum is lower than its associated minimum");

		max_ = max;
		min_ = min;
		step_ = step;	
	
	};


	EndCriteria::Type D6GridSearch::minimize(Problem& P,
		const EndCriteria& endCriteria) {

		// create the grid
		D6MultiGrid grid;

		for (Size i = 0; i < size_; i++)
			grid.addDimensionStep(i, min_[i], max_[i], step_[i]);

		std::vector<Array> guesses = grid.results();

		std::vector<double> res;

		for (std::vector<Array>::const_iterator It = guesses.cbegin();
			It != guesses.cend(); It++) {
		
			res.push_back(-P.value(*It));
		
		}
		
		// find the highest result
		Size t = std::distance(
			res.cbegin(), std::max_element(res.cbegin(), res.cend()));

		P.setCurrentValue(guesses[t]);

		// TODO: get the max value (quick sort) and associated array
		return EndCriteria::Type::StationaryFunctionValue;

	}

}


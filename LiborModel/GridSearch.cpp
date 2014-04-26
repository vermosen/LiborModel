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
		const Array & max,
		const Array & min,
		const Array & step) {
	
		QL_REQUIRE(max.size() == size_, "grid max values has wrong dimensions");
		QL_REQUIRE(max.size() == size_, "grid min values has wrong dimensions");
		QL_REQUIRE(max.size() == size_, "grid step values has wrong dimensions");

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

		opt_.push_back(boost::shared_ptr<OptimizationMethod>(
			new LevenbergMarquardt(epsfcn_, xtol_, gtol_)));

		// build initial values
		Array guess(1, 0.0);

		QuantLib::Problem local = P;
		local.value(guess);
		return opt_[0]->minimize(local, endCriteria);

	}

	void addParameterStep(
		Natural position,
		Real max,
		Real min,
		Real stepSize) {};

}


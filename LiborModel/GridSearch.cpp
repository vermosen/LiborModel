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

#include <ql/math/optimization/constraint.hpp>
#include <ql/math/optimization/lmdif.hpp>
#include <ql/math/optimization/levenbergmarquardt.hpp>
#include "gridsearch.hpp"
#include <boost/bind.hpp>

namespace QuantLib {

	MultiGrid::MultiGrid(Size size) : size_(size) {
	
		max_ = Array(size_, 0.0);
		min_ = Array(size_, 0.0);
		stepSize_ = Array(size_, 0.0);

	};

	void MultiGrid::addDimensionStep(
		Natural position,
		Real max,
		Real min,
		Real stepSize) {

		max_[position] = max;
		min_[position] = min;
		stepSize_[position] = stepSize;

	}

	GridSearch::GridSearch(
		Size size,
		Real epsfcn,
		Real xtol,
		Real gtol) : size_(size), epsfcn_(epsfcn), xtol_(xtol), gtol_(gtol) {}

	EndCriteria::Type GridSearch::minimize(Problem& P,
		const EndCriteria& endCriteria) {

		// create the full vector
		Size totalSize = 1;

		// create the grid
		for (Size i = 0; i < size_; i++)
		{

			totalSize = totalSize * (max_[i] - min_[i]) / stepSize_[i];

		}

		opt_.push_back(boost::shared_ptr<OptimizationMethod>(
			new LevenbergMarquardt(epsfcn_, xtol_, gtol_)));

		// build initial values
		Array guess(1, 0.0);

		QuantLib::Problem local = P;
		local.value(guess);
		opt_[0]->minimize(local, endCriteria);

	}

	void addParameterStep(
		Natural position,
		Real max,
		Real min,
		Real stepSize) {};

}


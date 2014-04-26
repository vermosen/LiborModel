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

	D6MultiGrid::D6MultiGrid() {
	
		// default to 0.0
		max_ = Array(size_, 0.0);
		min_ = Array(size_, 0.0);
		step_ = Array(size_, 0.0);

	};

	void D6MultiGrid::addDimensionStep(
		Natural position,
		Real max,
		Real min,
		Real step) {

		QL_ENSURE(position < size_, "Bondary index out of range");

		max_[position] = max;
		min_[position] = min;
		step_[position] = step;

	}

	std::vector<Array> D6MultiGrid::results() {
	
		std::vector<Array> data;

		Array d(size_, 0.0);

		// create the sizes
		for (Size i = 0; i < 6; i++)		
			d[i] = (max_[i] - min_[i]) / step_[i];
		
		// returns the grid
		for (Size i_0 = 0; i_0 < d[0]; i_0++){
		
			for (Size i_1 = 0; i_1 < d[1]; i_1++){

				for (Size i_2 = 0; i_2 < d[2]; i_2++){

					for (Size i_3 = 0; i_3 < d[3]; i_3++){

						for (Size i_4 = 0; i_4 < d[4]; i_4++){

							for (Size i_5 = 0; i_5 < d[5]; i_5++){

								Array temp(0.0, 6);

								temp[0] = min_[0] + i_0 * step_[0];
								temp[1] = min_[1] + i_0 * step_[1];
								temp[2] = min_[2] + i_0 * step_[2];
								temp[3] = min_[3] + i_0 * step_[3];
								temp[4] = min_[4] + i_0 * step_[4];
								temp[5] = min_[5] + i_0 * step_[5];

								data.push_back(temp);

							}

						}

					}


				}


			}
		
		}

		return data

	};

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


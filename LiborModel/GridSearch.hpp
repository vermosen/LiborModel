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

/*! \file GridSearch.hpp
\brief Grid Search Method
*/

#ifndef quantlib_optimization_grid_search_hpp
#define quantlib_optimization_grid_search_hpp

#include <ql/math/optimization/problem.hpp>
#include <ql/math/optimization/method.hpp>
#include "MultiGrid.hpp"

namespace QuantLib {

	//! 6D Grid Search optimization method
	/*! Levenberg-Marquardt based grid-search
	*/

	template <class T>
	class GridSearch: public OptimizationMethod {

	public:

		GridSearch(
			Real epsfcn = 1.0e-8,
			Real xtol = 1.0e-8,
			Real gtol = 1.0e-8)
			: grid_(T()), epsfcn_(epsfcn), xtol_(xtol), gtol_(gtol) {};

		virtual EndCriteria::Type minimize(Problem& P,
			const EndCriteria& endCriteria) {

			for (Size i = 0; i < grid_.size(); i++)
				grid_.addDimensionStep(i, min_[i], max_[i], step_[i]);

			std::vector<Array> guesses = grid_.results();

			std::vector<double> res; res.reserve(guesses.size());

			for (std::vector<Array>::const_iterator It = guesses.cbegin();
				It != guesses.cend(); It++) {

				try
				{
				
					double result = -P.value(*It);
					res.push_back(result);

					std::cout 
						<< "guess "
						<< *It
						<< " produced score of "
						<< result
						<< std::endl;
				
				}
				catch (std::exception & e)
				{

					res.push_back(-1000);

					std::cout
						<< "guess "
						<< *It
						<< " produced an error..."
						<< std::endl
						<< e.what()
						<< std::endl;

				}
				
			}

			// find the highest result
			Size t = std::distance(
				res.cbegin(), std::max_element(res.cbegin(), res.cend()));

			P.setCurrentValue(guesses[t]);

			// TODO: get the max value (quick sort) and associated array
			return EndCriteria::Type::StationaryFunctionValue;

		};

		void setGrid(
			const Array & min,
			const Array & max,
			const Array & step) {

			QL_REQUIRE(max.size() == grid_.size(), "grid max values has wrong dimensions");
			QL_REQUIRE(min.size() == grid_.size(), "grid min values has wrong dimensions");
			QL_REQUIRE(step.size() == grid_.size(), "grid step values has wrong dimensions");

			for (Size i = 0; i < grid_.size(); i++)
				QL_REQUIRE(max[i] >= min[i],
				"one of the maximum is lower than its associated minimum");

			max_ = max;
			min_ = min;
			step_ = step;

		};

	private:

		T grid_;

		Real epsfcn_;
		Real xtol_;
		Real gtol_;

		Problem* currentProblem_;
		Array max_;
		Array min_;
		Array step_;

		std::vector<Array> data_;

	};

}

#endif

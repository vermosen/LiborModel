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
#include <boost/multi_array.hpp>

namespace QuantLib {

	//! 6D Grid Search optimization method
	/*! Levenberg-Marquardt based grid-search
	*/

	class D6MultiGrid {

	public:

		D6MultiGrid();

		void addDimensionStep(
			Natural position,
			Real max,
			Real min,
			Real step);

		std::vector<Array> results();

	private:

		const Size size_ = 6;
		Array max_;
		Array min_;
		Array step_;

		std::vector<Array> D6MultiGrid::results();

	};

	class GridSearch: public OptimizationMethod {
	public:
		GridSearch(
			Size size,
			Real epsfcn = 1.0e-8,
			Real xtol = 1.0e-8,
			Real gtol = 1.0e-8);

		virtual EndCriteria::Type minimize(Problem& P,
			const EndCriteria& endCriteria);

	private:

		Size size_;
		Real epsfcn_;
		Real xtol_;
		Real gtol_;

		Problem* currentProblem_;
		Array max_;
		Array min_;
		Array stepSize_;

		std::vector<boost::shared_ptr<OptimizationMethod> > opt_;

	};

}


#endif

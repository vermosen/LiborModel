#ifndef model_construction_new
#define model_construction_new

#include <ql/models/marketmodels/all.hpp>
#include <ql/legacy/libormarketmodels/all.hpp>
#include <ql/utilities/csvbuilder.hpp>
#include <ql/models/shortrate/calibrationhelpers/swaptionhelper.hpp>
#include <ql/math/optimization/levenbergmarquardt.hpp>
#include <ql/math/randomnumbers/rngtraits.hpp>
#include <ql/methods/montecarlo/multipathgenerator.hpp>
#include <ql/math/randomnumbers/seedgenerator.hpp>

#include "curveCreation/curveCreation.hpp"

boost::shared_ptr<MarketModel> modelConstruction(
	boost::shared_ptr<IborIndex> &,
	const Natural &,
	utilities::csvBuilder &,
	bool);

#endif
#ifndef model_construction
#define model_construction

#include <ql/legacy/all.hpp>
#include <ql/utilities/csvbuilder.hpp>
#include <ql/models/shortrate/calibrationhelpers/swaptionhelper.hpp>
#include <ql/math/optimization/levenbergmarquardt.hpp>
#include <ql/math/randomnumbers/rngtraits.hpp>
#include <ql/methods/montecarlo/multipathgenerator.hpp>
#include <ql/math/randomnumbers/seedgenerator.hpp>

#include "curveCreation/curveCreation.hpp"

boost::shared_ptr<LiborForwardModel> modelConstruction(
	boost::shared_ptr<IborIndex> &,
	const Natural &,
	utilities::csvBuilder &,
	bool);

#endif
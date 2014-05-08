#ifndef european_swaption
#define european_swaption

#include <ql/legacy/all.hpp>
#include <ql/utilities/csvbuilder.hpp>
#include <ql/models/shortrate/calibrationhelpers/swaptionhelper.hpp>
#include <ql/math/optimization/levenbergmarquardt.hpp>
#include <ql/math/randomnumbers/rngtraits.hpp>
#include <ql/methods/montecarlo/multipathgenerator.hpp>
#include <ql/math/randomnumbers/seedgenerator.hpp>

#include "curveCreation/curveCreation.hpp"

void europeanSwaption(
	boost::shared_ptr<LiborForwardModel> &,
	boost::shared_ptr<IborIndex> &,
	utilities::csvBuilder &);

#endif
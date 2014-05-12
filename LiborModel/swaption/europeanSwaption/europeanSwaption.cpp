/*
*
* The goal of this project is to calibrate blabla...
*
*
*
*
*
*/

#include "swaption/europeanSwaption/europeanSwaption.hpp"

void europeanSwaption(
	boost::shared_ptr<LiborForwardModel> & lfm,
	boost::shared_ptr<IborIndex> & libor,
	utilities::csvBuilder & file) {

	Date pricingDate =									// pricing date
		Settings::instance().evaluationDate();

	Date fwdStart(16, July, 2017);
	Date fwdMaturity(16, July, 2020);
	//Date fwdMaturity = fwdStart + Period(3, Years);

	Schedule schedule(
		fwdStart, 
		fwdMaturity, 
		libor->tenor(), 
		libor->fixingCalendar(),
		ModifiedFollowing, 
		ModifiedFollowing, 
		DateGeneration::Backward, 
		false);

	Rate swapRate = 0.0404;								// dummy swap rate

	boost::shared_ptr<VanillaSwap> forwardSwap(
		new VanillaSwap(VanillaSwap::Receiver, 100.0,
		schedule, swapRate, ActualActual(),
		schedule, libor, 0.0, libor->dayCounter()));

	forwardSwap->setPricingEngine(boost::shared_ptr<PricingEngine>(
		new DiscountingSwapEngine(libor->forwardingTermStructure())));

	swapRate = forwardSwap->fairRate();										// obtain the fair rate

	// try to generate a smile
	for (Size i = 0; i < 1; i++) {
	
		forwardSwap = boost::shared_ptr<VanillaSwap>(		// rebuild the "right" swap
			new VanillaSwap(VanillaSwap::Receiver, 100.0,
			schedule, swapRate + .0025 * i, ActualActual(),
			schedule, libor, 0.0, libor->dayCounter()));
		forwardSwap->setPricingEngine(boost::shared_ptr<PricingEngine>(
			new DiscountingSwapEngine(libor->forwardingTermStructure())));

		boost::shared_ptr<PricingEngine> engine(
			new LfmSwaptionEngine(lfm,
			libor->forwardingTermStructure()));
		boost::shared_ptr<Exercise> exercise(
			new EuropeanExercise(fwdStart));

		boost::shared_ptr<Swaption> europeanSwaption(									// create the swaption
			new Swaption(forwardSwap, exercise));
		europeanSwaption->setPricingEngine(engine);

		Real npv = europeanSwaption->NPV();
		Real vol = europeanSwaption->impliedVolatility(	// assumed vol level
			npv, libor->forwardingTermStructure(), .48);

		std::cout << "European swaption npv: "											// information
			<< npv
			<< std::endl
			<< "implied vol is: "
			<< vol
			<< "(initial value was 48.900)"
			<< std::endl;
	
	}
}
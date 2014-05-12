/*
*
* The goal of this project is to calibrate blabla...
*
*
*
*
*
*/

#include "swaption/americanSwaption/americanSwaption.hpp"

void americanSwaption(
	boost::shared_ptr<LiborForwardModel> & lfm,
	boost::shared_ptr<IborIndex> & libor,
	utilities::csvBuilder & file) {

	Date pricingDate =									// pricing date
		Settings::instance().evaluationDate();

	Date optionStart = libor->fixingCalendar().advance(	// start in 2 days
		pricingDate, 
		Period(2, Days));
	
	Date optionEnd(16, July, 2016);
	Date fwdMaturity(16, July, 2021);

	//Date optionEnd = libor->fixingCalendar().advance(	// start in 2 days
	//	optionStart,
	//	Period(6, Months));

	//Date fwdMaturity = optionStart + Period(3, Years);	// underlying 3 years

	Schedule schedule(
		optionStart,
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

	swapRate = forwardSwap->fairRate();					// obtain the fair rate

	forwardSwap = boost::shared_ptr<VanillaSwap>(		// rebuild the "right" swap
		new VanillaSwap(VanillaSwap::Receiver, 100.0,
		schedule, swapRate, ActualActual(),
		schedule, libor, 0.0, libor->dayCounter()));
	forwardSwap->setPricingEngine(boost::shared_ptr<PricingEngine>(
		new DiscountingSwapEngine(libor->forwardingTermStructure())));

	boost::shared_ptr<PricingEngine> engine(
		new LfmSwaptionEngine(lfm,
		libor->forwardingTermStructure()));
	boost::shared_ptr<Exercise> exercise(
		new AmericanExercise(optionEnd));

	boost::shared_ptr<Swaption> americanSwaption(		// create the swaption
		new Swaption(forwardSwap, exercise));
	americanSwaption->setPricingEngine(engine);

	Real npv = americanSwaption->NPV();
	
	std::cout << "American swaption npv: "											// information
			  << npv
			  << std::endl;
			  
}
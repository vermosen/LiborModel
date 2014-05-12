/*
*
* The goal of this project is to calibrate blabla...
*
*
*
*
*
*/

#include "swaption/bermudeanSwaption/bermudeanSwaption.hpp"

void bermudeanSwaption(
	boost::shared_ptr<LiborForwardModel> & lfm,
	boost::shared_ptr<IborIndex> & libor,
	utilities::csvBuilder & file) {

	Date pricingDate =									// pricing date
		Settings::instance().evaluationDate();

	Date optionStart = libor->fixingCalendar().advance(	// start in 2 days
		pricingDate,
		Period(2, Days));

	Date last(16, July, 2016);
	Date fwdMaturity(16, July, 2021);

	std::vector<Date> dates;							// dates for payment

	while (last > optionStart) {
	
		dates.push_back(
			libor->fixingCalendar().adjust(
			last));
	
		last = libor->fixingCalendar().advance(
			last, Period(-3, Months));

	}

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
		new BermudanExercise(dates));

	boost::shared_ptr<Swaption> americanSwaption(		// create the swaption
		new Swaption(forwardSwap, exercise));
	americanSwaption->setPricingEngine(engine);

	Real npv = americanSwaption->NPV();
	
	std::cout << "Bermudean swaption npv: "											// information
			  << npv
			  << std::endl;
			  
}
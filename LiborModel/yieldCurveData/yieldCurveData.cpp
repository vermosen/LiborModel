/*
*
* The goal of this project is to calibrate blabla...
*
*
*
*
*
*/

#include "test1Y5/test1Y5.hpp"

void yieldCurveData() {

	// The final data to collect
	std::vector<std::string> names;
	std::vector<double> effectiveDates;
	std::vector<double> maturities;
	std::vector<double> quotes;
	std::vector<double> rates;

	const Real convexity_ = 0.009;						// convexity adjustment for future prices
	const Real meanReverting_ = 0.09;					// mean reversion for future prices

	const Calendar calendar								// pricing calendar
		= JointCalendar(
		UnitedStates(UnitedStates::Settlement),
		UnitedKingdom(UnitedKingdom::Settlement));

	const Date pricingDate								// set global evaluation date
		= Settings::instance().evaluationDate();

	/* create the yield term structure */
	boost::shared_ptr<RelinkableHandle<YieldTermStructure> > ts(
		new RelinkableHandle<YieldTermStructure>());

	boost::shared_ptr<IborIndex> libor(					// libor index associated
		new USDLibor(
		Period(3, Months),
		*ts));

	libor->addFixing(Date(16, April, 2014), .0022785);	// add a few fixings
	libor->addFixing(Date(15, April, 2014), .0022635);
	libor->addFixing(Date(14, April, 2014), .0022865);
	libor->addFixing(Date(11, April, 2014), .0022645);

	// the rate helpers
	std::vector<boost::shared_ptr<RateHelper> > rateHelpers;

	std::vector<depositData> depositRates				// deposit data
		= std::vector<depositData>
	{

		{ .0009070, 0, Period(1, Days) },
		{ .0014250, 1, Period(2, Days) },
		{ .0012150, 2, Period(1, Weeks) },
		{ .0015200, 2, Period(1, Months) },
		{ .0019300, 2, Period(2, Months) }

	};

	std::vector<futureData> futureRates					// future data
		= std::vector<futureData>
	{

		{ 99.7700, "EDM4" },
		{ 99.7600, "EDU4" },
		{ 99.7250, "EDZ4" },
		{ 99.6300, "EDH5" },
		{ 99.4550, "EDM5" },
		{ 99.2250, "EDU5" },
		{ 98.9450, "EDZ5" },
		{ 98.6450, "EDH6" },
		{ 98.3400, "EDM6" },
		{ 98.0500, "EDU6" },
		{ 97.7850, "EDZ6" }

	};

	std::vector<swapData> swapRates					// future data
		= std::vector<swapData>
	{

		{ .0139980, Period(4, Years) },
		{ .0176100, Period(5, Years) },
		{ .0205300, Period(6, Years) },
		{ .0228500, Period(7, Years) },
		{ .0247290, Period(8, Years) },
		{ .0262750, Period(9, Years) },
		{ .0275070, Period(10, Years) },
		{ .0286800, Period(11, Years) },
		{ .0296100, Period(12, Years) }

	};

	for (std::vector<depositData>::const_iterator It = depositRates.cbegin();
		It != depositRates.cend(); It++)
	{

		// settlement and maturity dates
		Date settlement = calendar.advance(
			pricingDate, Period(It->settlementDays_, Days));

		Date maturity = calendar.advance(
			settlement, It->maturity_);

		// creating the deposit
		boost::shared_ptr<QuantLib::deposit> myDepositPtr(
			new deposit(
			pricingDate,
			maturity,
			calendar,
			It->settlementDays_));

		Handle<Quote> quoteHandler(						// quote handle
			boost::shared_ptr<Quote>(
			new SimpleQuote(
			myDepositPtr->cleanPrice(
			It->quote_,
			Actual360(),
			Simple,
			Once,
			Unadjusted,
			settlement))));


		rateHelpers.push_back(							// insert RateHelper
			boost::shared_ptr<RateHelper>(
			new	depositBootstrapHelper(
			quoteHandler,
			myDepositPtr)));

		names.push_back("deposit");						// data
		effectiveDates.push_back(pricingDate.serialNumber());
		maturities.push_back(maturity.serialNumber());
		quotes.push_back(It->quote_);

	}

	for (std::vector<futureData>::const_iterator It = futureRates.cbegin();
		It != futureRates.cend(); It++)
	{

		Handle<Quote> convexityHandle(					// convexity adjustment
			boost::shared_ptr<Quote>(
			new SimpleQuote(convexity_)));

		Handle<Quote> meanRevertingHandle(				// mean reverting
			boost::shared_ptr<Quote>(
			new SimpleQuote(meanReverting_)));

		Handle<Quote> futurePriceHandle(				// price
			boost::shared_ptr<Quote>(
			new SimpleQuote(It->price_)));

		Date futureDate = IMM::date(					// IMM date
			It->futureCode_.substr(2, 2),
			pricingDate);

		Handle<Quote> convexityAdjustedQuoteHendle(		// convexity adjustement
			boost::shared_ptr<Quote>(
			new futuresConvexityAdjustmentQuote2(
			libor,
			futureDate,
			futurePriceHandle,
			convexityHandle,
			meanRevertingHandle,
			pricingDate)));

		rateHelpers.push_back(							// helper
			boost::shared_ptr<FuturesRateHelper>(
			new FuturesRateHelper(
			futurePriceHandle,
			futureDate,
			libor,
			convexityAdjustedQuoteHendle)));

		names.push_back("future");						// data
		effectiveDates.push_back(futureDate.serialNumber());
		maturities.push_back(
			calendar.advance(
				futureDate, Period(3, Months)).serialNumber());
		quotes.push_back(It->price_);
	
	}

	for (std::vector<swapData>::const_iterator It = swapRates.cbegin();
		It != swapRates.cend(); It++)
	{


		QuantLib::Handle<QuantLib::Quote> myHandle(			// creates the rate handle
			boost::shared_ptr<QuantLib::Quote>(
			new QuantLib::SimpleQuote(It->fairRate_)));


		rateHelpers.push_back(
			boost::shared_ptr<QuantLib::RateHelper>(new
			QuantLib::SwapRateHelper(
			myHandle,
			It->maturity_,
			calendar,
			Semiannual,
			ModifiedFollowing,
			ActualActual(),
			libor)));

		names.push_back("swap");							// data
		effectiveDates.push_back(pricingDate.serialNumber());
		maturities.push_back(
			calendar.advance(
				pricingDate, It->maturity_).serialNumber());
		quotes.push_back(It->fairRate_);

	}

	boost::shared_ptr<swapCurve> curve(						// building the curve object
		new swapCurve(rateHelpers, calendar, 10e-12));

	// create diagnostic file for the yield curve
	{

		std::string fileStr("C:/Temp/yield_curve_data_");	// build file path
		fileStr.append(boost::posix_time::to_iso_string(
			boost::posix_time::second_clock::local_time()));
		fileStr.append(".csv");

		utilities::csvBuilder file(fileStr);				// csv builder

		for (std::vector<double>::const_iterator It = maturities.cbegin(); 
			It != maturities.cend(); ++It)

			rates.push_back(curve->zeroRate(
				Date(*It), Actual365Fixed(), Continuous));

		// titles
		file.add("instrument Code", 1, 1);
		file.add("effective Date", 1, 2);
		file.add("Maturity", 1, 3);
		file.add("Quote", 1, 4);
		file.add("Underlying rate", 1, 5);

		// data
		file.add(names, 2, 1);
		file.add(effectiveDates, 2, 2);
		file.add(maturities, 2, 3);
		file.add(quotes, 2, 4);
		file.add(rates, 2, 5);

	}

}
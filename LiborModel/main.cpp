#include "main.hpp"

int main() {

	try
	{

		//

		const Date pricingDate = Date(16, April, 2014);		// pricing date

		Settings::instance().evaluationDate()				// set global evaluation date
			= pricingDate;

		/* basic settings */
		SavedSettings backup;

		std::string fileStr("C:/Temp/liborModel_1Y5_");		// build file path
		fileStr.append(boost::posix_time::to_iso_string(
			boost::posix_time::second_clock::local_time()));
		fileStr.append(".csv");

		utilities::csvBuilder file(fileStr);				// csv builder

		// step 1: curve creation
		std::cout << "step 1 : curve creation" << std::endl;
		std::cout << "-----------------------" << std::endl;

		boost::shared_ptr<IborIndex> curve = curveCreation();

		std::cout << "curve initialization completed." << std::endl;
		std::cout << std::endl;

		// step 2: vol term structure
		std::cout << "step 2 : Market Model Setup" << std::endl;
		std::cout << "---------------------------" << std::endl;

		std::cout << "please select the appropriate procudure:" << std::endl;
		std::cout << "1 - 10Y1 max maturity"                    << std::endl;
		std::cout << "2 - 5Y1 max maturity"                     << std::endl;
		std::cout << "3 - 5Y1 with default parameters"          << std::endl;
		std::cout << "4 - curve data check"                     << std::endl;
		
		int i;  std::cin >> i;

		boost::shared_ptr<LiborForwardModel> lfm;
		
		switch (i) {								// calling the test

			case 1:
				lfm = modelConstruction(curve, 11, file, false);
				break;
			case 2:
				lfm = modelConstruction(curve, 6, file, false);
				break;
			case 3:
				lfm = modelConstruction(curve, 6, file, true);
				break;
			case 4: 
				yieldCurveData();
				break;
			default:
				throw std::exception("wrong test selected");
		
		}

		// step 3 : model pricing
		europeanSwaption(lfm, curve, file);
		americanSwaption(lfm, curve, file);
		bermudeanSwaption(lfm, curve, file);

		system("pause");
		return 0;

	}
	catch (std::exception & e){

		std::cout << "an error occured: "
			<< std::endl;

		std::cout << e.what()
			<< std::endl;

		system("pause");
		return 1;

	}
	catch (...){

		std::cout << "an unknown error occured..."
			<< std::endl;

		system("pause");
		return 1;

	}
}
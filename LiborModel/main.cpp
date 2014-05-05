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

		int i;
		std::cout << "please select the appropriate test:" << std::endl;
		std::cout << "-----------------------------------" << std::endl;
		std::cout << "1 - 10Y1 max maturity" << std::endl;
		std::cout << "2 - 5Y1 max maturity" << std::endl;
		std::cout << "3 - 5Y1 grid version" << std::endl;
		std::cout << "4 - grid building test" << std::endl;
		std::cout << "5 - grid search test" << std::endl;
		std::cout << "6 - obtain yield curve data" << std::endl;

		std::cin >> i;

		/* calling the right test */
		switch (i) {

			case 1:
				test1Y10();
				break;
			case 2:
				test1Y5();
				break;
			case 3:
				test1Y5Grid();
				break;
			case 4:
				testGrid();
				break;
			case 5:
				testGridSearch();
				break;
			case 6:
				yieldCurveData();
				break;
			default:
				throw std::exception("wrong test selected");
		
		}

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
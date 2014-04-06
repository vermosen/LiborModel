#include <ql/quantlib.hpp>

int main() {

	QuantLib::UnitedStates calendar(QuantLib::UnitedStates::Settlement);

	std::cout << calendar.name() << std::endl;

	return 1;

}
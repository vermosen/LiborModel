#ifndef test_1y5_grid
#define test_1Y5_grid

#include <iostream>

#include <ql/legacy/all.hpp>
#include <ql/utilities/csvbuilder.hpp>
#include <ql/models/shortrate/calibrationhelpers/swaptionhelper.hpp>

#include "curveCreation/curveCreation.hpp"
#include "GridSearch.hpp"

void test1Y5Grid(const IborIndex &, bool);

#endif
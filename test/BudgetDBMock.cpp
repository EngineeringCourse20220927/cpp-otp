#include "BudgetDBMock.hpp"
#include <chrono>
#include "../main/date.h"

using namespace date;
using namespace std::chrono;

BudgetDBMock::BudgetDBMock() {
    date::year_month x = date::year(2019) / date::month(1);
    budgetList_.emplace_back(2019, 1, 31);
    budgetList_.emplace_back(2019, 2, 28);
    budgetList_.emplace_back(2019, 4, 30);
}

std::vector<IBudgetDB::Budget> BudgetDBMock::findAll() {
    return budgetList_;
}
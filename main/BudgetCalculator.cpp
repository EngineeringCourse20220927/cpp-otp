#include "BudgetCalculator.hpp"
#include <stdexcept>
#include <vector>
#include "IBudgetDB.hpp"
#include "date.h"

BudgetCalculator::BudgetCalculator(IBudgetDB &budgetDB) : budgetDB_(budgetDB) {
}

uint32_t BudgetCalculator::query(date::year_month_day const start, date::year_month_day const end) {
    if (start > end) {
        return 0;
    }

    std::vector<IBudgetDB::Budget> allBudget = budgetDB_.findAll();

    BudgetRange budgetRange = filterBudgetList(allBudget, start, end);

    if (budgetRange.end == allBudget.end() || budgetRange.start == allBudget.end()) {
        return 0;
    }

    date::year_month_day const lastDayOfFoundMonth = budgetRange.end->getEndDate();
    date::year_month_day const firstDayOfFoundMonth = budgetRange.start->getStartDate();
    date::year_month_day realEnd{};
    date::year_month_day realStart{};

    if (lastDayOfFoundMonth < end) {
        realEnd = lastDayOfFoundMonth;
    } else {
        realEnd = end;
    }

    if (firstDayOfFoundMonth > start) {
        realStart = firstDayOfFoundMonth;
    } else {
        realStart = start;
    }

    Period period{realStart, realEnd};
    auto budget = budgetRange.start;
    uint32_t budgetBetween = 0;
    while (budget != budgetRange.end + 1) {
        budgetBetween += budget->getDailyAmount() *
                         Period(budget->getStartDate() > period.getStart() ? budget->getStartDate() : period.getStart(),
                                budget->getEndDate() < period.getEnd() ? budget->getEndDate()
                                                                       : period.getEnd()).getDayCount();
        budget++;
    }

    return budgetBetween;
}

BudgetCalculator::BudgetRange
BudgetCalculator::filterBudgetList(std::vector<IBudgetDB::Budget> const &allBudget, date::year_month_day const &start,
                                   date::year_month_day const &end) noexcept {
    BudgetRange budgetRange;
    date::year_month startMonth = start.year() / start.month();
    date::year_month endMonth = end.year() / end.month();
    budgetRange.start = allBudget.end();
    budgetRange.end = allBudget.end();
    bool foundStart = false;
    std::vector<IBudgetDB::Budget>::const_iterator it = allBudget.begin();
    while (it != allBudget.end()) {
        if ((!foundStart) && (it->budgetMonth_ >= startMonth)) {
            budgetRange.start = it;
            foundStart = true;
        }

        if (it->budgetMonth_ <= endMonth) {
            budgetRange.end = it;
        } else {
            break;
        }
        it++;
    }
    return budgetRange;
}
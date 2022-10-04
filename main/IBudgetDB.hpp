#pragma once

#include <cstdint>
#include <vector>
#include "date.h"

class IBudgetDB {
public:
    class Budget {
    public:
        Budget(uint16_t year, uint8_t month, uint32_t money) : budgetMonth_(date::year(year) / date::month(month)),
                                                               money_(money) {
        }

        date::year_month budgetMonth_;
        uint32_t money_;

        uint32_t getDailyAmount() const noexcept {
            date::year_month_day const lastDay = budgetMonth_.year() / budgetMonth_.month() / date::last;
            date::year_month_day const firstDay = budgetMonth_.year() / budgetMonth_.month() / 1;
            return money_ / ((date::sys_days(lastDay) - date::sys_days(firstDay)).count() + 1);
        }

        date::year_month_day getEndDate() const noexcept {
            return budgetMonth_.year() / budgetMonth_.month() / date::last;
        }

    };

    ~IBudgetDB() noexcept = default;

    virtual std::vector<Budget> findAll() = 0; // fixme std::bad_alloc?

};
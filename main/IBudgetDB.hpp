#pragma once

#include <cstdint>
#include <vector>
#include "date.h"
#include "Period.h"

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
            return money_ / getPeriod().getDayCount();
        }

        Period getPeriod() const noexcept {
            return Period{budgetMonth_};
        }

        uint32_t getOverlappingAmount(const Period &period) const {
            return getDailyAmount() * period.getOverlappingDayCount(getPeriod());
        }

    };

    ~IBudgetDB() noexcept = default;

    virtual std::vector<Budget> findAll() = 0; // fixme std::bad_alloc?

};
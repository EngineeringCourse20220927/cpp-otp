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
    };

    ~IBudgetDB() noexcept = default;

    virtual std::vector<Budget> findAll() = 0; // fixme std::bad_alloc?
};
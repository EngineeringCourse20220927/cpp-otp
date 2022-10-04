#ifndef CPP_OTP_PERIOD_H
#define CPP_OTP_PERIOD_H

#include "date.h"

class Period {
public:
    Period(date::year_month_day const &start, date::year_month_day const &end) : start_(start), end_(end) {}

    Period(date::year_month const &yearMonth) : start_(yearMonth / 1), end_(yearMonth / date::last) {}

    int getDayCount() const noexcept {
        if (start_ > end_) {
            return 0;
        }
        return (date::sys_days(end_) - date::sys_days(start_)).count() + 1;
    }

    int getOverlappingDayCount(const Period &another) const {
        return Period(std::max(another.start_, start_), std::min(another.end_, end_)).getDayCount();
    }

private:
    date::year_month_day const start_;
    date::year_month_day const end_;
};

#endif //CPP_OTP_PERIOD_H

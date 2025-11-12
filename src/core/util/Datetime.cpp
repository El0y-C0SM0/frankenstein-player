#include "core/util/Datetime.hpp"

namespace core {
    Datetime::Datetime() {
        timePoint = std::chrono::system_clock::now();
    }

    Datetime::Datetime(const std::string& datetimeStr) {
        int len = datetimeStr.length();
        if(len < 8 || len > 10) {
            timePoint = std::chrono::system_clock::now();
            return;
        }

        std::tm t = {};
        int ano;
        std::stringstream ss(datetimeStr);
        ss >> t.tm_mday;
        ss >> t.tm_mon;
        ss >> ano;
        ano -= 1900;
        t.tm_year = ano;

        std::time_t tt = std::mktime(&t);
        timePoint = std::chrono::system_clock::from_time_t(tt);
    }

    const std::chrono::system_clock::time_point& Datetime::getTimePoint() const {
        return timePoint;
    }

    const std::string Datetime::toString() const {
        std::string format = "%Y/%m/%d";
        std::time_t tt = std::chrono::system_clock::to_time_t(timePoint);
        std::tm* localTm = std::localtime(&tt);

        std::stringstream ss;
        ss << std::put_time(localTm, format.c_str());
        return ss.str();
    }

    bool Datetime::operator==(const Datetime& other) const {
        return this->toString() == other.toString();
    }

    bool Datetime::operator!=(const Datetime& other) const {
        return !(*this == other);
    }

    bool Datetime::isBefore(const Datetime& other) const {
        if((*this) == other) return false;
        return this->getTimePoint() < other.getTimePoint();

    }

    bool Datetime::isAfter(const Datetime& other) const {
        if((*this) == other) return false;
        return this->getTimePoint() > other.getTimePoint();
    }
}
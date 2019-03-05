#ifndef SG_PROBING_MANAGER_HPP

#include "measurable.hpp"

#include <algorithm>
#include <list>
#include <map>
#include <memory>

namespace smokygrill {

class Probe {

public:

    inline std::list<MeasurableType_t> getCapabilities() const noexcept {
        std::list<MeasurableType_t> type_list;
        std::for_each(type_list.cbegin(), type_list.cend(), [&type_list](MeasurableTypePair_t pair){
           type_list.push_back(pair.first);
        });
        return type_list;
    }

    inline bool isCapableOf(MeasurableType_t type) const noexcept {
        return this->measurables.find(type) != this->measurables.end();
    }

    inline std::shared_ptr<Measurable> getMeasurable(MeasurableType_t type) const {
        return this->measurables.at(type);
    }

private:

    typedef std::pair<MeasurableType_t, std::shared_ptr<Measurable>> MeasurableTypePair_t;

    std::map<MeasurableType_t, std::shared_ptr<Measurable>> measurables;

};

} // namespace smokygrill

#endif

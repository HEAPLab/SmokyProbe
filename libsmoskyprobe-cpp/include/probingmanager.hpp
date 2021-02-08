#ifndef SG_PROBING_MANAGER_HPP

#include "probe.hpp"

#include <list>
#include <string>

namespace smokygrill {

class ProbingManager final {

public:

    ProbingManager(const std::string device);

    ~ProbingManager();

    inline const std::list<Probe> &getProbes() noexcept {
        return this->probe_list;
    }



private:

    std::list<Probe> probe_list;

};

} // namespace probing manager


#endif

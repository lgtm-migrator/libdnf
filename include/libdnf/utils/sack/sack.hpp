/*
Copyright (C) 2020 Red Hat, Inc.

This file is part of libdnf: https://github.com/rpm-software-management/libdnf/

Libdnf is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

Libdnf is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with libdnf.  If not, see <https://www.gnu.org/licenses/>.
*/


#ifndef LIBDNF_UTILS_SACK_SACK_HPP
#define LIBDNF_UTILS_SACK_SACK_HPP

#include "set.hpp"

#include "libdnf/utils/weak_ptr.hpp"

#include <vector>

namespace libdnf::utils::sack {


template <typename T, typename QueryT>
class Sack {
public:
    using DataItemWeakPtr = WeakPtr<T, false>;

    // EXCLUDES

    const Set<DataItemWeakPtr> & get_excludes() const noexcept { return excludes; }
    void add_excludes(const Set<DataItemWeakPtr> & value) { excludes.update(value); }
    void remove_excludes(const Set<DataItemWeakPtr> & value) { excludes.difference(value); }
    void set_excludes(const Set<DataItemWeakPtr> & value) { excludes = value; }

    // INCLUDES

    const Set<DataItemWeakPtr> & get_includes() const noexcept { return includes; }
    void add_includes(const Set<DataItemWeakPtr> & value) { includes.update(value); }
    void remove_includes(const Set<DataItemWeakPtr> & value) { includes.difference(value); }
    void set_includes(const Set<DataItemWeakPtr> & value) { includes = value; }
    bool get_use_includes() const noexcept { return use_includes; }
    void set_use_includes(bool value) { use_includes = value; }

    QueryT new_query();

protected:
    Sack() = default;
    WeakPtrGuard<T, false> data_guard;
    std::vector<T> & get_data() { return data; }
    Set<DataItemWeakPtr> excludes;
    Set<DataItemWeakPtr> includes;
    bool use_includes = false;

private:
    std::vector<T> data;  // Owns the data set. Objects get deleted when the Sack is deleted.
};

template <typename T, typename QueryT>
QueryT Sack<T, QueryT>::new_query() {
    QueryT result;
    for (auto & it : this->get_data()) {
        result.add(DataItemWeakPtr(&it, &this->data_guard));
    }

    // if includes are used, remove everything else from the query
    if (this->get_use_includes()) {
        result.intersection(this->includes);
    }

    // apply excludes
    result.difference(this->excludes);

    return result;
}


}  // namespace libdnf::utils::sack


#endif

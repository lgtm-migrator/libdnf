/*
Copyright Contributors to the libdnf project.

This file is part of libdnf: https://github.com/rpm-software-management/libdnf/

Libdnf is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 2.1 of the License, or
(at your option) any later version.

Libdnf is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with libdnf.  If not, see <https://www.gnu.org/licenses/>.
*/


#include "libdnf-cli/output/advisoryinfo.hpp"

#include "libdnf/advisory/advisory_collection.hpp"
#include "libdnf/advisory/advisory_reference.hpp"


namespace libdnf::cli::output {


void AdvisoryInfo::add_advisory(libdnf::advisory::Advisory & advisory) {
    add_line("Name", advisory.get_name(), "bold");
    add_line("Title", advisory.get_title());
    add_line("Severity", advisory.get_severity());
    add_line("Type", advisory.get_type());
    add_line("Status", advisory.get_status());
    add_line("Vendor", advisory.get_vendor());
    add_line("Buildtime", std::to_string(advisory.get_buildtime()));
    add_line("Description", advisory.get_description());
    add_line("Message", advisory.get_message());
    add_line("Rights", advisory.get_rights());

    // References
    for (auto reference : advisory.get_references()) {
        auto group_reference = add_line("Reference", "");
        add_line("Title", reference.get_title(), "bold", group_reference);
        add_line("Id", reference.get_id(), nullptr, group_reference);
        add_line("Type", reference.get_type_cstring(), nullptr, group_reference);
        add_line("Url", reference.get_url(), nullptr, group_reference);
    }

    // Collections
    for (auto collection : advisory.get_collections()) {
        auto group_collection = add_line("Collection", "");

        // Modules
        auto modules = collection.get_modules();
        if (!modules.empty()) {
            auto module_iter = modules.begin();
            add_line("Modules", module_iter->get_nsvca(), nullptr, group_collection);
            module_iter++;
            while (module_iter != modules.end()) {
                add_line("", module_iter->get_nsvca(), nullptr, group_collection);
                module_iter++;
            }
        }

        // Packages
        auto packages = collection.get_packages();
        if (!packages.empty()) {
            auto package_iter = packages.begin();
            add_line("Pacakges", package_iter->get_nevra(), nullptr, group_collection);
            package_iter++;
            while (package_iter != packages.end()) {
                add_line("", package_iter->get_nevra(), nullptr, group_collection);
                package_iter++;
            }
        }
    }
}

}  // namespace libdnf::cli::output
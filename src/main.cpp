#include <iostream>

#include <cstdlib>

#include "sanepp.h"

int main() {
    create_sane_instance();
    auto sane_instance = get_sane_instance();

    if (sane_instance) {
        auto device_list = sane_instance->get_devices();

        for (auto device_info : device_list) {
            std::cout << device_info.name() << std::endl;

            auto device = device_info.open_this_device();

            if (device)  {
                // is valid
                for (auto &option_entry : *device) {
                    std::cout << option_entry.second->description().name() << std::endl;
                }
            }
        }
    }

    return EXIT_SUCCESS;
}

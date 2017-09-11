#include <iostream>
#include <thread>
#include <string>
#include <chrono>

#include "sanepp.h"

int main() {
    sane::authorization_callback([](const std::string &, std::string &, std::string &) { });

    const sane &sane_instance = sane::instance();

    if (sane_instance) {
        auto device_list = sane_instance.devices();

        for (sane_device_info &device_info : device_list) {
            std::cout << "device found : " << device_info.name() << std::endl;

            auto device = device_info.open();

            if (device)  {
                // is valid
                for (auto &option_entry : device->options()) {

                    if (option_entry.description().name().find("Button") != std::string::npos) {

                        const sane_option_value &button_value = option_entry.value();
                        if (button_value.as_int()) {
                            std::cout << "found option : " << option_entry.description().name() << std::endl;
                            for (int i = 0; i < 10; i++) {
                                std::cout << "value : " << *(button_value.as_int()) << std::endl;
                                std::this_thread::sleep_for(std::chrono::milliseconds(20));
                            }

                        }
                    }
                }
            }
        }
    }

    return EXIT_SUCCESS;
}

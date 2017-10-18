#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "sanepp.h"

int main() {
    using namespace sanepp;

    Sane::authorization_callback([](const std::string &, std::string &, std::string &) {});

    Sane sane_instance;

    if (sane_instance) {
        auto device_list = sane_instance.devices();

        for (DeviceInfo &device_info : device_list) {
            std::cout << "device found : " << device_info.name() << std::endl;

            auto device = device_info.open();

            if (device) {
                // is valid
                for (auto &option_entry : device->options()) {
                    if (option_entry.info().name().find("button") != std::string::npos) {
                        std::cout << option_entry.info().name() << std::endl;
                        auto button_value = option_entry.value<int>();
                        if (button_value) {
                            for (int i = 0; i < 10; i++) {
                                std::cout << "value : " << *option_entry.value<int>() << std::endl;
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

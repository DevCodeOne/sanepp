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

            std::cout << "\ndevice found!" << std::endl;
            std::cout << "Device name: " << device_info.name() << std::endl;
            std::cout << "Device model: " << device_info.model() << std::endl;
            std::cout << "Device type: " << device_info.type() << std::endl;
            std::cout << "Device vendor: " << device_info.vendor() << std::endl;
            auto device = device_info.open();

            if (device) {
               std::cout << "\nOptions: " << std::endl;
                for (auto &option_entry : device->options()) {
                    std::cout << "Option ID: " << option_entry.info().id() << std::endl;
                    std::cout << "Option Name: " << option_entry.info().name() << std::endl;
                    std::cout << "Option Titel: " << option_entry.info().title() << std::endl;
                    std::cout << "Option Description: " << option_entry.info().description() << std::endl;
                }
            }
        }
    }

    return EXIT_SUCCESS;
}

#include <iostream>
#include <string>
#include <fstream>
#include <limits>  // Required for std::numeric_limits
#include <iomanip>
#include <vector>

int main() {
    std::cout << "Enter file path: ";
    std::string path;
    std::getline(std::cin, path);  // Initial path input

    while (path != "quit" && path != "Quit") {
        std::fstream file(path, std::fstream::in);
        if (!file.is_open()) {
            std::cout << "Could not locate file. Try again: ";
            std::getline(std::cin, path);
            if (path == "quit" || path == "Quit") {
                break;
            }
        }
        std::vector<double> all_temps{};

        std::string to_search {"bucketTouchTemperature"};
        std::string line {};
        while(std::getline(file, line)) {
            size_t init_find = line.find(to_search);
            if(init_find != std::string::npos) {
                //Find the colon after bucketTouchTemperature
                size_t num_start = line.find(':', init_find + to_search.length());
                if(num_start != std::string::npos) {
                    ++num_start; //Move to the start of the number
                    while(num_start < line.length() && std::isspace(line.at(num_start))) {
                        ++num_start;
                    }
                    size_t num_end = num_start;
                    //Go to the end of the number which is when a ',' or '}' is detected
                    while(num_end < line.length() && line.at(num_end) != ',' && line.at(num_end) != '}') {
                        ++num_end;
                    }

                    std::string number = line.substr(num_start, num_end - num_start);
                    try {
                        double temp = std::stod(number);
                        all_temps.push_back(temp);
                    }
                    catch(const std::invalid_argument& e) {
                        std::cerr << "Failed to get bucket temperature: " << e.what() << '\n';
                    }
                    catch(const std::out_of_range& e) {
                        std::cerr << "Temperature is out of range for some reason :/" << '\n';
                    }
                    catch(...) {
                        std::cerr << "A really weird exception was thrown. . .\n";
                    }
                }
            }
        }

        if(!all_temps.empty()) {
            double highest_temp = all_temps.at(0);
            double lowest_temp = all_temps.at(0);
            size_t above_threshold {0};
            for(const auto t : all_temps) {
                if(highest_temp < t) highest_temp = t;
                if(lowest_temp > t) lowest_temp = t;
                if(t > 125) ++above_threshold;
            }
            std::cout << std::setfill('=') << std::setw(10);
            std::cout << " RESULTS " << std::setfill('=') << std::setw(10) << '\n';
            std::cout << "Highest temp: " << highest_temp << '\n';
            std::cout << "Lowest temp: " << lowest_temp << '\n';
            std::cout << "Amount of temp readings above threshold: " << above_threshold << '\n';
            std::cout << "Total temp readings: " << all_temps.size();
        }
        else {
            std::cout << "No temperature data found.\n";
        }

        file.close();  // Close the file after reading
        file.clear();  // Clear any error flags from the file stream

        std::cout << "\nEnter another file or \"quit\" to exit: ";
        std::getline(std::cin, path);  // Request new input
    }

    return 0;
}
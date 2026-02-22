#include <iostream>
#include <vector>
#include <string>
#include <limits> // Added for input buffer clearing

class Appliance {
public:
    std::string name;
    double powerRating; // watts
    double dailyUsageHours;

    Appliance(std::string n, double pr, double hours) 
        : name(n), powerRating(pr), dailyUsageHours(hours) {}

    double calculateEnergyConsumption() const {
        return (powerRating * dailyUsageHours) / 1000.0; // kWh
    }
};

int main() {
    std::vector<Appliance> appliances;
    int choice;

    do {
        std::cout << "\n--- Electrical Load Monitoring ---\n";
        std::cout << "1. Register appliance\n";
        std::cout << "2. View appliances\n";
        std::cout << "3. Calculate total energy consumption\n";
        std::cout << "4. Exit\n";
        std::cout << "Enter choice: ";
        
        // Basic check to ensure a number was entered
        if (!(std::cin >> choice)) {
            std::cout << "Invalid input! Please enter a number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
        case 1: {
            std::string name;
            double powerRating, dailyUsageHours;

            std::cin.ignore(); // Clear the newline from the previous cin
            std::cout << "Enter appliance name (e.g., Washing Machine): ";
            std::getline(std::cin, name);

            std::cout << "Enter power rating (Watts): ";
            std::cin >> powerRating;
            std::cout << "Enter daily usage (Hours): ";
            std::cin >> dailyUsageHours;

            if (name.empty() || powerRating <= 0 || dailyUsageHours < 0 || dailyUsageHours > 24) {
                std::cout << "Invalid input! Power must be > 0 and Hours must be 0-24.\n";
            } else {
                appliances.emplace_back(name, powerRating, dailyUsageHours);
                std::cout << "Appliance added successfully!\n";
            }
            break;
        }
        case 2:
            if (appliances.empty()) {
                std::cout << "No appliances registered yet.\n";
            } else {
                std::cout << "\nRegistered Appliances:\n";
                for (const auto& app : appliances) {
                    std::cout << "- " << app.name << ": " << app.powerRating 
                              << "W, " << app.dailyUsageHours << " hrs/day\n";
                }
            }
            break;
        case 3: {
            double totalEnergy = 0;
            std::cout << "\nEnergy Consumption Report:\n";
            for (const auto& app : appliances) {
                // FIXED: Corrected the massive typo in the method call
                double energy = app.calculateEnergyConsumption();
                totalEnergy += energy;
                std::cout << " * " << app.name << ": " << energy << " kWh/day\n";
            }
            std::cout << "----------------------------\n";
            std::cout << "Total Daily Consumption: " << totalEnergy << " kWh\n";
            break;
        }
        case 4:
            std::cout << "Exiting...\n";
            break;
        default:
            std::cout << "Invalid choice! Please pick 1-4.\n";
        }
    } while (choice != 4);

    return 0;
}

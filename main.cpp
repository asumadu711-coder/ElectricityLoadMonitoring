#include <iostream>
#include <vector>
#include <string>

class Appliance {
public:
    std::string name;
    double powerRating; // watts
    double dailyUsageHours;

    Appliance(std::string n, double pr, double hours) : name(n), powerRating(pr), dailyUsageHours(hours) {}

    double calculateEnergyConsumption() {
        return (powerRating * dailyUsageHours) / 1000.0; // kWh
    }
};

int main() {
    std::vector<Appliance> appliances;
    int choice;

    do {
        std::cout << "Electrical Load Monitoring\n";
        std::cout << "1. Register appliance\n";
        std::cout << "2. View appliances\n";
        std::cout << "3. Exit\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1: {
            std::string name;
            double powerRating, dailyUsageHours;
            std::cout << "Enter appliance name, power rating (W), daily usage hours: ";
            std::cin >> name >> powerRating >> dailyUsageHours;
            if (name.empty() || powerRating <= 0 || dailyUsageHours < 0 || dailyUsageHours > 24) {
                std::cout << "Invalid input! Try again.\n";
                break;
            }
            appliances.push_back(Appliance(name, powerRating, dailyUsageHours));
            break;
        }
        case 2:
            std::cout << "Registered appliances:\n";
            for (const auto& app : appliances) {
                std::cout << "Name: " << app.name << ", Power: " << app.powerRating << "W, Hours: " << app.dailyUsageHours << "\n";
            }
            break;
        case 3:
            std::cout << "Exiting...\n";
            break;
        default:
            std::cout << "Invalid choice!\n";
        }
    } while (choice != 3);

    return 0;
}
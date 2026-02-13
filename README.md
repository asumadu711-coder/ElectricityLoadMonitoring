#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <limits>

using namespace std;

const string APPLIANCES_FILE = "appliances.txt";
const string BILLING_FILE = "billing_summary.txt";

// ------------------ Appliance Class ------------------
class Appliance {
public:
    string name;
    double powerWatts;
    double hoursPerDay;

    Appliance() : name(""), powerWatts(0), hoursPerDay(0) {}

    Appliance(string n, double p, double h)
        : name(n), powerWatts(p), hoursPerDay(h) {}

    double energyKWh() const {
        // Energy = (Watts / 1000) * hours
        return (powerWatts / 1000.0) * hoursPerDay;
    }
};

// ------------------ Helper Functions ------------------
string toLowerCase(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

double getValidatedDouble(const string& prompt, double minValue, double maxValue, bool hasMax) {
    double value;
    while (true) {
        cout << prompt;
        cin >> value;

        if (cin.fail()) {
            cout << "Invalid input. Please enter a number.\n";
            clearInput();
            continue;
        }

        if (value <= minValue) {
            cout << "Value must be greater than " << minValue << ". Try again.\n";
            continue;
        }

        if (hasMax && value > maxValue) {
            cout << "Value must be less than or equal to " << maxValue << ". Try again.\n";
            continue;
        }

        clearInput();
        return value;
    }
}

string getValidatedName(const string& prompt) {
    string name;
    while (true) {
        cout << prompt;
        getline(cin, name);

        if (name.empty()) {
            cout << "Appliance name must not be empty. Try again.\n";
            continue;
        }
        return name;
    }
}

// ------------------ File Handling ------------------
void saveAppliancesToFile(const vector<Appliance>& appliances) {
    ofstream fout(APPLIANCES_FILE);

    if (!fout) {
        cout << "Error: Could not save appliances to file.\n";
        return;
    }

    // Save as: name|watts|hours
    for (const auto& a : appliances) {
        fout << a.name << "|" << a.powerWatts << "|" << a.hoursPerDay << "\n";
    }

    fout.close();
    cout << "Appliances saved successfully to " << APPLIANCES_FILE << "\n";
}

void loadAppliancesFromFile(vector<Appliance>& appliances) {
    ifstream fin(APPLIANCES_FILE);

    if (!fin) {
        // File may not exist the first time. That is OK.
        return;
    }

    appliances.clear();

    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;

        size_t p1 = line.find('|');
        size_t p2 = line.find('|', p1 + 1);

        if (p1 == string::npos || p2 == string::npos) continue;

        string name = line.substr(0, p1);
        double watts = stod(line.substr(p1 + 1, p2 - (p1 + 1)));
        double hours = stod(line.substr(p2 + 1));

        appliances.push_back(Appliance(name, watts, hours));
    }

    fin.close();
}

// ------------------ Appliance Features ------------------
void registerAppliance(vector<Appliance>& appliances) {
    cout << "\n--- Register Appliance ---\n";

    string name = getValidatedName("Enter appliance name: ");

    double watts = getValidatedDouble(
        "Enter power rating in watts (>0): ",
        0, 0, false
    );

    // hours must be between 0 and 24 (0 allowed, but rule says between 0 and 24)
    // We'll allow 0, but not negative, and max 24.
    double hours;
    while (true) {
        cout << "Enter daily usage hours (0 - 24): ";
        cin >> hours;

        if (cin.fail()) {
            cout << "Invalid input. Enter a number.\n";
            clearInput();
            continue;
        }

        if (hours < 0 || hours > 24) {
            cout << "Usage hours must be between 0 and 24.\n";
            continue;
        }

        clearInput();
        break;
    }

    appliances.push_back(Appliance(name, watts, hours));
    cout << "Appliance registered successfully!\n";
}

void viewAppliances(const vector<Appliance>& appliances) {
    cout << "\n--- Registered Appliances ---\n";

    if (appliances.empty()) {
        cout << "No appliances registered yet.\n";
        return;
    }

    cout << left << setw(20) << "Name"
         << setw(15) << "Power(W)"
         << setw(15) << "Hours/Day"
         << setw(15) << "Energy(kWh)\n";

    cout << string(65, '-') << "\n";

    for (const auto& a : appliances) {
        cout << left << setw(20) << a.name
             << setw(15) << fixed << setprecision(2) << a.powerWatts
             << setw(15) << a.hoursPerDay
             << setw(15) << a.energyKWh()
             << "\n";
    }
}

void searchAppliance(const vector<Appliance>& appliances) {
    cout << "\n--- Search Appliance ---\n";

    if (appliances.empty()) {
        cout << "No appliances registered yet.\n";
        return;
    }

    string searchName = getValidatedName("Enter appliance name to search: ");
    string searchLower = toLowerCase(searchName);

    bool found = false;

    for (const auto& a : appliances) {
        if (toLowerCase(a.name).find(searchLower) != string::npos) {
            if (!found) {
                cout << "\nMatches Found:\n";
                cout << left << setw(20) << "Name"
                     << setw(15) << "Power(W)"
                     << setw(15) << "Hours/Day"
                     << setw(15) << "Energy(kWh)\n";
                cout << string(65, '-') << "\n";
            }

            found = true;
            cout << left << setw(20) << a.name
                 << setw(15) << fixed << setprecision(2) << a.powerWatts
                 << setw(15) << a.hoursPerDay
                 << setw(15) << a.energyKWh()
                 << "\n";
        }
    }

    if (!found) {
        cout << "No appliance found with that name.\n";
    }
}

// ------------------ Energy & Billing ------------------
double computeTotalEnergy(const vector<Appliance>& appliances) {
    double total = 0;
    for (const auto& a : appliances) {
        total += a.energyKWh();
    }
    return total;
}

void showEnergySummary(const vector<Appliance>& appliances) {
    cout << "\n--- Load & Energy Summary ---\n";

    if (appliances.empty()) {
        cout << "No appliances registered yet.\n";
        return;
    }

    viewAppliances(appliances);

    double totalEnergy = computeTotalEnergy(appliances);
    cout << "\nTotal daily energy consumption: "
         << fixed << setprecision(3) << totalEnergy << " kWh\n";
}

void saveBillingSummaryToFile(const vector<Appliance>& appliances, double tariff, double totalEnergy, double totalCost) {
    ofstream fout(BILLING_FILE);

    if (!fout) {
        cout << "Error: Could not save billing summary.\n";
        return;
    }

    fout << "====== BILLING SUMMARY ======\n\n";
    fout << left << setw(20) << "Name"
         << setw(15) << "Power(W)"
         << setw(15) << "Hours/Day"
         << setw(15) << "Energy(kWh)\n";

    fout << string(65, '-') << "\n";

    for (const auto& a : appliances) {
        fout << left << setw(20) << a.name
             << setw(15) << fixed << setprecision(2) << a.powerWatts
             << setw(15) << a.hoursPerDay
             << setw(15) << a.energyKWh()
             << "\n";
    }

    fout << "\nTotal daily energy: " << fixed << setprecision(3) << totalEnergy << " kWh\n";
    fout << "Tariff: " << fixed << setprecision(2) << tariff << " per kWh\n";
    fout << "Total cost: " << fixed << setprecision(2) << totalCost << "\n";

    fout.close();
}

void billingCalculation(const vector<Appliance>& appliances) {
    cout << "\n--- Billing Calculation ---\n";

    if (appliances.empty()) {
        cout << "No appliances registered yet.\n";
        return;
    }

    double tariff = getValidatedDouble(
        "Enter electricity tariff per kWh (positive): ",
        0, 0, false
    );

    double totalEnergy = computeTotalEnergy(appliances);
    double totalCost = totalEnergy * tariff;

    cout << "\n====== BILLING SUMMARY ======\n";
    cout << "Total daily energy: " << fixed << setprecision(3) << totalEnergy << " kWh\n";
    cout << "Tariff: " << fixed << setprecision(2) << tariff << " per kWh\n";
    cout << "Total electricity cost: " << fixed << setprecision(2) << totalCost << "\n";

    // Save billing summary
    saveBillingSummaryToFile(appliances, tariff, totalEnergy, totalCost);
    cout << "Billing summary saved to " << BILLING_FILE << "\n";
}

// ------------------ Menu ------------------
void showMenu() {
    cout << "\n==============================\n";
    cout << " Electrical Load Monitoring App\n";
    cout << "==============================\n";
    cout << "1. Register appliance\n";
    cout << "2. View all appliances\n";
    cout << "3. Search appliance by name\n";
    cout << "4. Load & energy summary\n";
    cout << "5. Billing calculation\n";
    cout << "6. Save appliances to file\n";
    cout << "7. Exit\n";
    cout << "Choose an option: ";
}

int main() {
    vector<Appliance> appliances;

    // Load from file at start (Requirement 5.4)
    loadAppliancesFromFile(appliances);

    int choice;

    while (true) {
        showMenu();
        cin >> choice;

        if (cin.fail()) {
            cout << "Invalid input. Please enter a number.\n";
            clearInput();
            continue;
        }

        clearInput();

        switch (choice) {
        case 1:
            registerAppliance(appliances);
            break;

        case 2:
            viewAppliances(appliances);
            break;

        case 3:
            searchAppliance(appliances);
            break;

        case 4:
            showEnergySummary(appliances);
            break;

        case 5:
            billingCalculation(appliances);
            break;

        case 6:
            saveAppliancesToFile(appliances);
            break;

        case 7:
            // Auto-save before exiting (Good practice)
            saveAppliancesToFile(appliances);
            cout << "Exiting program... Goodbye!\n";
            return 0;

        default:
            cout << "Invalid menu choice. Try again.\n";
        }
    }
}

Which one will go into the main.cpp,README.md,appliances.txt and billing_sammary.txt

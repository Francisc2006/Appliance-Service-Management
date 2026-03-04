# FixItNow - Home Appliance Repair Service Management & Simulation

## Project Description
FixItNow is a robust Object-Oriented application developed in Modern C++. It simulates the daily operations and management of an authorized home appliance repair service. The project handles the complete workflow: from staff validation and hiring, managing the catalog of supported appliances, to the real-time simulation of processing repair requests using a workload balancing algorithm.

## Main Features

* **Staff Management:** Administer different types of employees (Receptionists, Technicians, Supervisors). The system includes strict data validation (e.g., mathematical validation of the Romanian CNP, checking the minimum legal hiring age of 16) and dynamically calculates salaries by applying management bonuses, loyalty bonuses, and transport allowances.
* **Appliance Catalog:** Dynamic management of supported appliance brands and models (Refrigerators, TVs, Washing Machines).
* **Real-Time Simulation:** Once the simulation is started, the application processes requests "second by second" (ticks). Requests are automatically assigned to competent technicians based on an algorithm that checks their availability (max. 3 simultaneous requests) and balances the overall workload among the team.
* **Reporting (CSV Export):** Generates detailed reports directly into `.csv` files (Top 3 Salaries, Technician with the longest repair time, Pending requests grouped by category).

## Architecture and Technologies

This project strictly adheres to **Object-Oriented Programming (OOP)** and **SOLID** principles, serving as a clear demonstration of **Modern C++** practices:

* **Design Patterns:**
  * **Singleton:** Used for the central `Service` class, ensuring a single, globally accessible instance for overall application management.
  * **Factory Method:** Implemented via `AngajatFactory` and `FactoryElectrocasnic` for the clean instantiation of derived classes.
* **Modern C++:** Extensive use of **Smart Pointers** (`std::shared_ptr`, `std::unique_ptr`) to ensure memory safety and prevent leaks, alongside **Lambda functions** for complex sorting operations.
* **STL (Standard Template Library):** Choosing the right data structures for specific scenarios: `std::vector` (for static lists/frequent iterations), `std::list` (for fast deletions in the middle of active requests), `std::deque` (for the waiting queue), and `std::map` (for fast statistics).

---

## Build and Run Instructions

To compile and run the application locally, you will need a C++ compiler (e.g., `g++` from MinGW on Windows, or GCC/Clang on Linux/macOS).

**Step 1: Compile the source code**
Open your terminal in the project's root folder and run:
```bash
g++ *.cpp -o FixItNow
```
**Step 2: Run the application**
```bash
# On Windows:
FixItNow.exe

# On Linux / macOS:
./FixItNow
```
## Usage Guide and Testing

### How to run a test:
1. From the main menu, choose **Option 1 (Incarcare Scenariu / Load Scenario)**.
2. Enter the path to the desired test file (e.g., `tests/test1.txt`). *Note: Make sure the path is correct relative to the executable.*
3. Return to the main menu and select **Option 4 (Procesare Cereri / Process Requests)**, then press **1 (Ruleaza Simulare / Run Simulation)**.
4. Input the desired number of ticks (seconds) to watch how repairs are distributed and completed in real-time.

### Included Test Scenarios:
* **`test1.txt` (Ideal Scenario):** Demonstrates the correct functioning of the simulation, the fair distribution of requests, and strict adherence to technician competencies.
* **`test2.txt` (Employee Errors / Fail-Safe):** Tests strict input validations (invalid CNP, age under 16). It triggers the safety lock: the simulation will not start if the minimum staff requirement is not met (3 Technicians, 1 Receptionist, 1 Supervisor).
* **`test3.txt` (Request Errors):** Validates the rejection of repair requests for unsupported appliances or invalid complexity levels. Errors are logged into the unrepairable appliances statistic.
* **`test4.txt` (Low Load Scenario):** Demonstrates the efficiency of the workload balancing algorithm when the number of available employees vastly exceeds the volume of incoming repair requests.

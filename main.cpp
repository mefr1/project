#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <ctime>
#include "WorkoutTracker.h"
#include <sstream>
#include <climits>

const std::string FILE_NAME = "workouts.txt";

int safeIntInput(const std::string& prompt, int minVal = INT_MIN, int maxVal = INT_MAX) {
    int value;
    while (true) {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);
        std::stringstream ss(input);
        if (ss >> value && !(ss >> input) && value >= minVal && value <= maxVal)
            return value;
        std::cout << "❌ Неверный ввод. Повторите.\n";
    }
}

float safeFloatInput(const std::string& prompt, float minVal = -1e9f, float maxVal = 1e9f) {
    float value;
    while (true) {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);
        std::stringstream ss(input);
        if (ss >> value && !(ss >> input) && value >= minVal && value <= maxVal)
            return value;
        std::cout << "❌ Неверный ввод. Повторите.\n";
    }
}

std::string getCurrentDate() {
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);
    char buffer[11];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", localTime);
    return std::string(buffer);
}

std::shared_ptr<Exercise> createExerciseFromInput() {
    int type = safeIntInput("1 - Силовое\n2 - Кардио\n3 - С собственным весом\nВаш выбор: ", 1, 3);

    std::string name;
    std::cout << "Название упражнения: ";
    std::getline(std::cin, name);

    if (type == 1) {
        auto ex = std::make_shared<StrengthExercise>(name);
        std::cout << "Введите подходы (0 в любом поле — завершение):\n";
        while (true) {
            float weight = safeFloatInput("  Вес: ", 0);
            if (weight <= 0) break;

            int reps = safeIntInput("  Повторения: ", 1);
            ex->addSet(reps, weight);
        }
        return ex;
    } else if (type == 2) {
        int duration = safeIntInput("Длительность (мин): ", 1);
        return std::make_shared<CardioExercise>(name, duration);
    } else if (type == 3) {
        auto ex = std::make_shared<BodyweightExercise>(name);
        std::cout << "Введите подходы (0 — завершить):\n";
        while (true) {
            int reps = safeIntInput("  Повторы: ", 0);
            if (reps <= 0) break;
            ex->addSet(reps);
        }
        return ex;
    }
    std::cout << "Неверный тип упражнения.\n";
    return nullptr;
}

void addWorkoutSession(WorkoutTracker& tracker) {
    std::string date = getCurrentDate();
    std::cout << "\n=== Начало тренировки на сегодня: " << date << " ===\n";

    Workout workout(date);

    while (true) {
        std::cout << "\nВыберите действие:\n1. Добавить упражнение\n0. Завершить тренировку\n";
        int choice = safeIntInput("Ваш выбор: ", 0, 1);

        if (choice == 0) break;

        if (choice == 1) {
            auto ex = createExerciseFromInput();
            if (ex) {
                workout.addExercise(ex);
                std::cout << "Упражнение добавлено!\n";
            }
        }
    }

    tracker.addWorkout(workout);
    tracker.saveToFile(FILE_NAME);
    std::cout << "✅ Тренировка успешно сохранена.\n";
}

void showMenu() {
    std::cout << "\n=== Меню ===\n1. Начать тренировку\n2. Показать все тренировки\n3. Показать статистику\n0. Выход\nВыбор: ";
}

int main() {
    WorkoutTracker tracker;
    tracker.loadFromFile(FILE_NAME);

    while (true) {
        showMenu();
        int choice = safeIntInput("", 0, 3);

        switch (choice) {
            case 1:
                addWorkoutSession(tracker);
                break;
            case 2:
                tracker.showAllWorkouts();
                break;
            case 3:
                tracker.printStatistics();
                break;
            case 0:
                std::cout << "Выход из программы\n";
                return 0;
        }
    }
}

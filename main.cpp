#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <ctime>
#include "WorkoutTracker.h"
#include <limits>
const std::string FILE_NAME = "workouts.txt";

std::string getCurrentDate() {
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);
    char buffer[11];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", localTime);
    return std::string(buffer);
}

std::shared_ptr<Exercise> createExerciseFromInput() {
    int type;
    std::cout << "1 - Силовое\n2 - Кардио\n3 - С собственным весом\n";
    std::cin >> type;
    std::cin.ignore();

    std::string name;
    std::cout << "Название упражнения: ";
    std::getline(std::cin, name);

    if (type == 1) {
        auto ex = std::make_shared<StrengthExercise>(name);
        std::cout << "Введите подходы (0 в любом поле — завершение):\n";
while (true) {
    float weight;
    std::cout << "  Вес: ";
    std::cin >> weight;
    if (weight <= 0) {

        break;
    }

    int reps;
    std::cout << "  Повторения: ";
    std::cin >> reps;
    if (reps <= 0) {

        continue;
    }

    ex->addSet(reps, weight);
}
        std::cin.ignore();
        return ex;
    } else if (type == 2) {
        int duration;
        std::cout << "Длительность (мин): ";
        std::cin >> duration;
        std::cin.ignore();
        return std::make_shared<CardioExercise>(name, duration);
    }
    else if (type == 3) {
    auto ex = std::make_shared<BodyweightExercise>(name);
    std::cout << "Введите подходы (0 — завершить):\n";
    while (true) {
        int reps;
        std::cout << "  Повторы: ";
        std::cin >> reps;
        if (reps <= 0) break;
        ex->addSet(reps);
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return ex;
}
    std::cout << "Неверный тип упражнения.\n";
    return nullptr;
}

void addWorkoutSession(WorkoutTracker& tracker) {
    std::string date = getCurrentDate();
    std::cout << "\n";
    std::cout << "=== Начало тренировки на сегодня: " << date << " ===\n";

    Workout workout(date);

    while (true) {
        std::cout << "\nВыберите действие:\n";
        std::cout << "1. Добавить упражнение\n";
        std::cout << "0. Завершить тренировку\n";
        std::cout << "Ваш выбор: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 0) break;

        if (choice == 1) {
            auto ex = createExerciseFromInput();
            if (ex) {
                workout.addExercise(ex);
                std::cout << "Упражнение добавлено!\n";
            }
        } else {
            std::cout << "Неверный ввод. Попробуйте снова.\n";
        }
    }

    tracker.addWorkout(workout);
    tracker.saveToFile(FILE_NAME);
    std::cout << "✅ Тренировка успешно сохранена.\n";
}



void showMenu() {
    std::cout << "\n=== Меню ===\n";
    std::cout << "1. Начать тренировку\n";
    std::cout << "2. Показать все тренировки\n";
    std::cout << "3. Показать статистику\n";
    std::cout << "0. Выход\n";
    std::cout << "Выбор: ";
}

int main() {
    WorkoutTracker tracker;
    tracker.loadFromFile(FILE_NAME);

    while (true) {
        showMenu();

        int choice;
        std::cin >> choice;

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
            default:
                std::cout << "Неверный вариант\n";
        }
    }
}

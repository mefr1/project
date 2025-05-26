#include <iostream>
#include <memory>
#include "WorkoutTracker.h"

void showMenu() {
    std::cout << "\n=== Workout Tracker Menu ===\n";
    std::cout << "1. Добавить тренировку\n";
    std::cout << "2. Показать все тренировки\n";
    std::cout << "0. Выход\n";
    std::cout << "Выбор: ";
}

std::shared_ptr<Exercise> createExerciseFromInput() {
    int type;
    std::cout << "Выберите тип упражнения (1 - силовое, 2 - кардио): ";
    std::cin >> type;
    std::cin.ignore();

    std::string name;
    std::cout << "Название упражнения: ";
    std::getline(std::cin, name);

    if (type == 1) {
        auto ex = std::make_shared<StrengthExercise>(name);

        std::cout << "\nВведите подходы. Введите 0 для повтора или веса, чтобы завершить.\n";
        while (true) {
            int reps;
            float weight;
            std::cout << "  Повторы: ";
            std::cin >> reps;
            std::cout << "  Вес (кг): ";
            std::cin >> weight;

            if (reps <= 0 || weight <= 0) {
                std::cout << "  Завершение ввода подходов...\n";
                break;
            }

            ex->addSet(reps, weight);
        }

        return ex;

    } else if (type == 2) {
        int duration;
        std::cout << "Длительность (мин): ";
        std::cin >> duration;
        return std::make_shared<CardioExercise>(name, duration);
    } else {
        std::cout << "Неверный тип. Пропуск...\n";
        return nullptr;
    }
}



void addWorkoutSession(WorkoutTracker& tracker) {
    std::cin.ignore();
    std::string date;
    std::cout << "=== Начало новой тренировки ===\n";
    std::cout << "Введите дату тренировки (например, 2025-05-26): ";
    std::getline(std::cin, date);

    Workout workout(date);

    while (true) {
        std::cout << "\nДобавить упражнение или завершить тренировку:\n";
        std::cout << "1. Добавить упражнение\n";
        std::cout << "0. Завершить тренировку\n";
        std::cout << "Выбор: ";
        int choice;
        std::cin >> choice;

        if (choice == 0) {
            break;
        } else if (choice == 1) {
            auto ex = createExerciseFromInput();
            if (ex) {
                workout.addExercise(ex);
                std::cout << "Упражнение добавлено!\n";
            }
        } else {
            std::cout << "Неверный выбор.\n";
        }
    }

    tracker.addWorkout(workout);
    std::cout << "Тренировка сохранена!\n";
}


int main() {
    WorkoutTracker tracker;
    int choice;

    while (true) {
        showMenu();
        std::cin >> choice;

        switch (choice) {
        case 1:
            addWorkoutSession(tracker);
            break;
        case 2:
            tracker.showAllWorkouts();
            break;
        case 0:
            std::cout << "До свидания!\n";
            return 0;
        default:
            std::cout << "Неверный ввод. Попробуйте снова.\n";
        }
    }

    return 0;
}

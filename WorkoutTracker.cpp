#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>
#include "WorkoutTracker.h"
#include <map>
void WorkoutTracker::addWorkout(const Workout& w) {
    workouts.push_back(w);
}

void WorkoutTracker::showAllWorkouts() const {
    for (const auto& w : workouts) {
        w.showWorkout();
        std::cout << "------------------\n";
    }

}


void WorkoutTracker::saveToFile(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "Error: could not open file for writing.\n";
        return;
    }

    for (const auto& workout : workouts) {
        out << "=== Date: " << workout.getDate() << " ===\n";

        for (const auto& ex : workout.getExercises()) {
            if (auto se = std::dynamic_pointer_cast<StrengthExercise>(ex)) {
                out << "Strength Exercise: " << se->getName() << "\n";
                const auto& sets = se->getSets();
                for (size_t i = 0; i < sets.size(); ++i) {
                    out << "  Set " << i + 1 << ": " << sets[i].reps
                        << " reps, " << sets[i].weight << " kg\n";
                }
            } else if (auto ce = std::dynamic_pointer_cast<CardioExercise>(ex)) {
                out << "Cardio Exercise: " << ce->getName()
                    << " - " << ce->getDuration() << "\n";
            }
            else if (auto be = std::dynamic_pointer_cast<BodyweightExercise>(ex)) {
    out << "Bodyweight Exercise: " << be->getName() << "\n";
    const auto& sets = be->getSets();
    for (size_t i = 0; i < sets.size(); ++i) {
        out << "  Set " << i + 1 << ": " << sets[i] << " reps\n";
    }
}
        }

        out << "\n";
    }

    std::cout << "✅ Saved to file: " << filename << "\n";
}


void WorkoutTracker::loadFromFile(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) {
        std::cerr << "⚠ File not found: " << filename << ". Starting with empty list.\n";
        return;
    }

    std::string line;
    Workout* currentWorkout = nullptr;

    while (std::getline(in, line)) {

        if (line.rfind("=== Date: ", 0) == 0) {
            std::string date = line.substr(10);
            if (date.rfind(" ===") != std::string::npos)
                date.erase(date.length() - 4);
            workouts.emplace_back(date);
            currentWorkout = &workouts.back();
        }


        else if (line.rfind("Strength Exercise: ", 0) == 0) {
            std::string name = line.substr(19);
            auto ex = std::make_shared<StrengthExercise>(name);


            while (std::getline(in, line) && line.rfind("  Set", 0) == 0) {
                try {
                    size_t colonPos = line.find(':');
                    size_t commaPos = line.find(',', colonPos);
                    size_t kgPos = line.find("kg", commaPos);

                    if (colonPos == std::string::npos || commaPos == std::string::npos || kgPos == std::string::npos)
                        throw std::invalid_argument("invalid format");

                    std::string repsStr = line.substr(colonPos + 2, commaPos - colonPos - 2);
                    std::string weightStr = line.substr(commaPos + 2, kgPos - commaPos - 2);

                    int reps = std::stoi(repsStr);
                    float weight = std::stof(weightStr);

                    ex->addSet(reps, weight);
                } catch (const std::exception& e) {
                    std::cerr << "⚠ Failed to parse strength set: " << line << " (" << e.what() << ")\n";
                }
            }

            if (currentWorkout) currentWorkout->addExercise(ex);


            if (!in.eof() && !line.empty()) {
                in.seekg(-static_cast<int>(line.length()) - 1, std::ios_base::cur);
            }
        }


        else if (line.rfind("Cardio Exercise: ", 0) == 0) {
            size_t sep = line.find(" — ");
            if (sep == std::string::npos)
                sep = line.find(" - ");

            if (sep != std::string::npos) {
                std::string name = line.substr(17, sep - 17);
                try {
                    std::string durationStr = line.substr(sep + 3);
                    int duration = std::stoi(durationStr);
                    auto ex = std::make_shared<CardioExercise>(name, duration);
                    if (currentWorkout) currentWorkout->addExercise(ex);
                } catch (const std::exception& e) {
                    std::cerr << "⚠ Failed to parse cardio duration: " << line << " (" << e.what() << ")\n";
                }
            } else {
                std::cerr << "⚠ Failed to parse cardio exercise: " << line << "\n";
            }
        }

        else if (line.rfind("Bodyweight Exercise: ", 0) == 0) {
    std::string name = line.substr(21); // всё после заголовка
    auto ex = std::make_shared<BodyweightExercise>(name);

    while (std::getline(in, line) && line.rfind("  Set", 0) == 0) {
        try {
            size_t colonPos = line.find(':');
            size_t repsPos = line.find(" reps", colonPos);
            std::string repsStr = line.substr(colonPos + 2, repsPos - colonPos - 2);
            int reps = std::stoi(repsStr);
            ex->addSet(reps);
        } catch (...) {
            std::cerr << "⚠ Ошибка при разборе подхода с собственным весом: " << line << "\n";
        }
    }

    if (currentWorkout) currentWorkout->addExercise(ex);

    if (!in.eof() && !line.empty()) {
        in.seekg(-static_cast<int>(line.length()) - 1, std::ios_base::cur);
    }
}
        else if (line.empty()) {
            continue;
        }


        else {
            std::cerr << "⚠ Unknown line: " << line << "\n";
        }
    }

    std::cout << "✅ Successfully loaded from file: " << filename << "\n";
}


void WorkoutTracker::printStatistics() const {
    std::map<std::string, float> maxWeightPerExercise;
    std::map<std::string, int> maxRepsPerBodyweightExercise;



    std::cout << "\n=== Статистика по тренировкам ===\n";

    for (const auto& workout : workouts) {
        float totalStrengthWeight = 0.0f;
        int totalCardioTime = 0;

        for (const auto& ex : workout.getExercises()) {
            if (auto se = std::dynamic_pointer_cast<StrengthExercise>(ex)) {
                for (const auto& set : se->getSets()) {
                    totalStrengthWeight += set.reps * set.weight;

                    float& currentMax = maxWeightPerExercise[se->getName()];
                    if (set.weight > currentMax)
                        currentMax = set.weight;


                }
            } else if (auto ce = std::dynamic_pointer_cast<CardioExercise>(ex)) {
                totalCardioTime += ce->getDuration();
            }
           else if (auto be = std::dynamic_pointer_cast<BodyweightExercise>(ex)) {
    for (int reps : be->getSets()) {
        int& currentMax = maxRepsPerBodyweightExercise[be->getName()];
        if (reps > currentMax)
            currentMax = reps;
    }
}
        }

        std::cout << "\nДата: " << workout.getDate() << "\n";
        std::cout << "  🔸 Общий поднятый вес: " << totalStrengthWeight << " кг\n";
        std::cout << "  🔸 Общее время кардио: " << totalCardioTime << " минут\n";
    }

    std::cout << "\n🏋️ Максимальный вес по силовым упражнениям:\n";
for (const auto& [name, maxW] : maxWeightPerExercise) {
    std::cout << "  💪 " << name << " — " << maxW << " кг\n";
}

std::cout << "\n🤸 Максимальные повторы по упражнениям с собственным весом:\n";
for (const auto& [name, maxReps] : maxRepsPerBodyweightExercise) {
    std::cout << "  🧍 " << name << " — " << maxReps << " повторений\n";
}

}

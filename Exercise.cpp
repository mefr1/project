#include "Exercise.h"

Exercise::Exercise(const std::string& n) : name(n) {}

// --- Силовое ---
StrengthExercise::StrengthExercise(const std::string& n)
    : Exercise(n) {}

void StrengthExercise::addSet(int reps, float weight) {
    sets.emplace_back(reps, weight);
}

void StrengthExercise::display() const {
    std::cout << "Strength: " << name << "\n";
    for (size_t i = 0; i < sets.size(); ++i) {
        std::cout << "  Set " << i + 1
                  << " | Reps: " << sets[i].reps
                  << " | Weight: " << sets[i].weight << " kg\n";
    }
}

// --- Кардио ---
CardioExercise::CardioExercise(const std::string& n, int duration)
    : Exercise(n), durationMinutes(duration) {}

void CardioExercise::display() const {
    std::cout << "Cardio: " << name
              << " | Duration: " << durationMinutes << " min\n";
}

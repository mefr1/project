#include "Exercise.h"

// ---- базовый и силовой ----
Exercise::Exercise(const std::string& n, int s, int r)
    : name(n), sets(s), reps(r) {}

StrengthExercise::StrengthExercise(const std::string& n, int s, int r, float w)
    : Exercise(n, s, r), weight(w) {}

void StrengthExercise::display() const {
    std::cout << "Strength: " << name
              << " | Sets: " << sets
              << " | Reps: " << reps
              << " | Weight: " << weight << " kg\n";
}

// ---- кардио ----
CardioExercise::CardioExercise(const std::string& n, int duration)
    : Exercise(n), durationMinutes(duration) {}

void CardioExercise::display() const {
    std::cout << "Cardio: " << name
              << " | Duration: " << durationMinutes << " min\n";
}

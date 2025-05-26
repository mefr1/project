#include "Exercise.h"
#include <iostream>

// --- StrengthExercise ---

StrengthExercise::StrengthExercise(const std::string& name)
    : Exercise(name) {}

std::string StrengthExercise::getType() const {
    return "Strength";
}

void StrengthExercise::display() const {
    std::cout << "Strength: " << name << "\n";
    for (size_t i = 0; i < sets.size(); ++i) {
        std::cout << "  Set " << i + 1 << " | Reps: " << sets[i].reps
                  << " | Weight: " << sets[i].weight << " kg\n";
    }
}

void StrengthExercise::addSet(int reps, float weight) {
    sets.push_back({reps, weight});
}

const std::vector<StrengthSet>& StrengthExercise::getSets() const {
    return sets;
}

// --- CardioExercise ---

CardioExercise::CardioExercise(const std::string& name, int duration)
    : Exercise(name), duration(duration) {}

std::string CardioExercise::getType() const {
    return "Cardio";
}

void CardioExercise::display() const {
    std::cout << "Cardio: " << name << " — " << duration << " min\n";
}

int CardioExercise::getDuration() const {
    return duration;
}

// --- BodyweightExercise ---

BodyweightExercise::BodyweightExercise(const std::string& name)
    : Exercise(name) {}

std::string BodyweightExercise::getType() const {
    return "Bodyweight";
}

void BodyweightExercise::display() const {
    std::cout << "Bodyweight: " << name << "\n";
    for (size_t i = 0; i < sets.size(); ++i) {
        std::cout << "  Set " << i + 1 << " | Reps: " << sets[i] << "\n";
    }
}

void BodyweightExercise::addSet(int reps) {
    sets.push_back(reps);
}

const std::vector<int>& BodyweightExercise::getSets() const {
    return sets;
}

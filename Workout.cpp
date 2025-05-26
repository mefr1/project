#include "Workout.h"
#include <iostream>

Workout::Workout(const std::string& d) : date(d) {}

void Workout::addExercise(std::shared_ptr<Exercise> ex) {
    exercises.push_back(ex);
}

void Workout::showWorkout() const {
    std::cout << "Workout Date: " << date << '\n';
    for (const auto& ex : exercises)
        ex->display();
}

std::string Workout::getDate() const {
    return date;
}

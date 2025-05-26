#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Exercise.h"

class Workout {
    std::string date;
    std::vector<std::shared_ptr<Exercise>> exercises;

public:
    Workout(const std::string& d);
    void addExercise(std::shared_ptr<Exercise> ex);
    void showWorkout() const;
    std::string getDate() const;
    const std::vector<std::shared_ptr<Exercise>>& getExercises() const;
};

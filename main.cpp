#include <iostream>
#include "WorkoutTracker.h"

int main() {
    WorkoutTracker tracker;

    Workout w1("2025-05-26");
    w1.addExercise(std::make_shared<StrengthExercise>("Bench Press", 3, 8, 60));
    w1.addExercise(std::make_shared<CardioExercise>("Running", 30));

    Workout w2("2025-05-27");
    w2.addExercise(std::make_shared<StrengthExercise>("Deadlift", 3, 5, 100));
    w2.addExercise(std::make_shared<CardioExercise>("Cycling", 45));

    tracker.addWorkout(w1);
    tracker.addWorkout(w2);

    tracker.showAllWorkouts();

    return 0;
}

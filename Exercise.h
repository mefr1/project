#pragma once
#include <string>
#include <iostream>

class Exercise {
protected:
    std::string name;
    int sets;
    int reps;

public:
    Exercise(const std::string& n, int s = 0, int r = 0); // sets и reps по умолчанию
    virtual void display() const = 0;
    virtual ~Exercise() {}
};

class StrengthExercise : public Exercise {
    float weight;

public:
    StrengthExercise(const std::string& n, int s, int r, float w);
    void display() const override;
};

class CardioExercise : public Exercise {
    int durationMinutes;

public:
    CardioExercise(const std::string& n, int duration);
    void display() const override;
};

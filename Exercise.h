#pragma once
#include <string>
#include <iostream>
#include <vector>

struct SetInfo {
    int reps;
    float weight;

    SetInfo(int r, float w) : reps(r), weight(w) {}
};

class Exercise {
protected:
    std::string name;

public:
    Exercise(const std::string& n);
    virtual void display() const = 0;
    virtual ~Exercise() {}
};

class StrengthExercise : public Exercise {
    std::vector<SetInfo> sets;

public:
    StrengthExercise(const std::string& n);
    void addSet(int reps, float weight);
    void display() const override;

    const std::vector<SetInfo>& getSets() const;
    const std::string& getName() const;
};

class CardioExercise : public Exercise {
    int durationMinutes;

public:
    CardioExercise(const std::string& n, int duration);
    void display() const override;

    int getDuration() const;
    const std::string& getName() const;
};

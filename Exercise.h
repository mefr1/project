#ifndef EXERCISE_H
#define EXERCISE_H

#include <string>
#include <vector>
#include <memory>

class Exercise {
protected:
    std::string name;
public:
    Exercise(const std::string& name) : name(name) {}
    virtual ~Exercise() = default;

    virtual std::string getType() const = 0;
    virtual void display() const = 0;

    const std::string& getName() const { return name; }
};


struct StrengthSet {
    int reps;
    float weight;
};

class StrengthExercise : public Exercise {
private:
    std::vector<StrengthSet> sets;
public:
    StrengthExercise(const std::string& name);

    std::string getType() const override;
    void display() const override;

    void addSet(int reps, float weight);
    const std::vector<StrengthSet>& getSets() const;
};


class CardioExercise : public Exercise {
private:
    int duration;
public:
    CardioExercise(const std::string& name, int duration);

    std::string getType() const override;
    void display() const override;

    int getDuration() const;
};


class BodyweightExercise : public Exercise {
private:
    std::vector<int> sets;
public:
    BodyweightExercise(const std::string& name);

    std::string getType() const override;
    void display() const override;

    void addSet(int reps);
    const std::vector<int>& getSets() const;
};

#endif

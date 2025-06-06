
#include <stdio.h>
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <ctime>
#include <fstream>
#include <map>

#include "WorkoutTracker.h"
#include "Exercise.h"


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

static std::string getCurrentDate()
{
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);
    char buffer[11];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", localTime);
    return std::string(buffer);
}

enum class AppScreen
{
    MainMenu,
    AddWorkout,
    ShowAllWorkouts,
    Statistics
};

struct NewSetInput {
    int reps    = 0;
    float weight = 0.0f;
};

struct AddExerciseGUI {
    bool inProgress = false;

    int type = 0;
    char name[128] = "";


    std::vector<NewSetInput> strengthSets;
    int newReps = 0;
    float newWeight = 0.0f;


    int cardioDuration = 0;


    std::vector<int> bwSets;
    int newBwReps = 0;
};

struct AppState {
    AppScreen screen = AppScreen::MainMenu;
    WorkoutTracker tracker;
    Workout currentWorkout{ "" };

    AddExerciseGUI newEx;
};



static void ShowMainMenu(AppState& state)
{
    ImGui::Begin("Главное меню");

    ImGui::Text("Трекер тренировок");

    if (ImGui::Button("1. Начать тренировку")) {
        state.screen = AppScreen::AddWorkout;
        state.currentWorkout = Workout(getCurrentDate());
        state.newEx = AddExerciseGUI();
    }

    if (ImGui::Button("2. Показать все тренировки")) {
        state.screen = AppScreen::ShowAllWorkouts;
    }

    if (ImGui::Button("3. Показать статистику")) {
        state.screen = AppScreen::Statistics;
    }

    if (ImGui::Button("0. Выход")) {
        glfwSetWindowShouldClose(glfwGetCurrentContext(), GLFW_TRUE);
    }

    ImGui::End();
}

static void ShowAddWorkout(AppState& state)
{
    ImGui::Begin("Добавление тренировки");

    ImGui::Text("Дата тренировки: %s", state.currentWorkout.getDate().c_str());
    ImGui::Separator();

    if (!state.currentWorkout.getExercises().empty()) {
        ImGui::Text("Добавленные упражнения:");
        int idx = 0;
        for (const auto& ex : state.currentWorkout.getExercises()) {
            char header[64];
            snprintf(header, sizeof(header), "%d) %s", ++idx, ex->getName().c_str());
            if (ImGui::TreeNode(header)) {
                std::string type = ex->getType();
                if (type == "Strength") {
                    auto se = std::dynamic_pointer_cast<StrengthExercise>(ex);
                    for (auto& s : se->getSets()) {
                        ImGui::BulletText("Сил. подход: %d повторений, %.1f кг", s.reps, s.weight);
                    }
                }
                else if (type == "Cardio") {
                    auto ce = std::dynamic_pointer_cast<CardioExercise>(ex);
                    ImGui::BulletText("Кардио: %d мин", ce->getDuration());
                }
                else if (type == "Bodyweight") {
                    auto be = std::dynamic_pointer_cast<BodyweightExercise>(ex);
                    for (int rep : be->getSets()) {
                        ImGui::BulletText("Собств. вес подход: %d повторений", rep);
                    }
                }
                ImGui::TreePop();
            }
        }
        ImGui::Separator();
    }

    if (!state.newEx.inProgress)
    {
        ImGui::Text("Добавить новое упражнение:");

        const char* types[] = { "Strength", "Cardio", "Bodyweight" };
        ImGui::Combo("Тип упражнения", &state.newEx.type, types, IM_ARRAYSIZE(types));
        ImGui::InputText("Название упражнения", state.newEx.name, IM_ARRAYSIZE(state.newEx.name));

        if (strlen(state.newEx.name) > 0) {
            if (ImGui::Button("Начать упражнение")) {
                state.newEx.inProgress = true;
                state.newEx.strengthSets.clear();
                state.newEx.newReps = 0;
                state.newEx.newWeight = 0.0f;
                state.newEx.bwSets.clear();
                state.newEx.newBwReps = 0;
                state.newEx.cardioDuration = 0;
            }
        }
    }
    else
    {
        ImGui::Text("Упражнение: %s", state.newEx.name);
        int exType = state.newEx.type;

        if (exType == 0)
        {
            ImGui::Text("Добавление силового подхода:");
            ImGui::InputInt("Повторы", &state.newEx.newReps);
            ImGui::InputFloat("Вес (кг)", &state.newEx.newWeight);
            if (state.newEx.newReps > 0 && state.newEx.newWeight > 0.0f) {
                if (ImGui::Button("Добавить подход")) {
                    state.newEx.strengthSets.push_back(
                        NewSetInput{ state.newEx.newReps, state.newEx.newWeight }
                    );
                    state.newEx.newReps = 0;
                    state.newEx.newWeight = 0.0f;
                }
            }
            ImGui::Separator();
            if (!state.newEx.strengthSets.empty()) {
                ImGui::Text("Добавленные подходы:");
                int i = 0;
                for (auto& s : state.newEx.strengthSets) {
                    ImGui::BulletText("%d) %d повторений, %.1f кг", ++i, s.reps, s.weight);
                }
            }
            ImGui::Separator();
            if (!state.newEx.strengthSets.empty()) {
                if (ImGui::Button("Завершить упражнение")) {
                    auto se = std::make_shared<StrengthExercise>(state.newEx.name);
                    for (auto& s : state.newEx.strengthSets) {
                        se->addSet(s.reps, s.weight);
                    }
                    state.currentWorkout.addExercise(se);
                    state.newEx = AddExerciseGUI();
                }
                ImGui::SameLine();
                if (ImGui::Button("Отменить упражнение")) {
                    state.newEx = AddExerciseGUI();
                }
            }
        }
        else if (exType == 1)
        {
            ImGui::Text("Добавление кардио:");
            ImGui::InputInt("Длительность (мин)", &state.newEx.cardioDuration);
            if (state.newEx.cardioDuration > 0) {
                if (ImGui::Button("Добавить упражнение")) {
                    auto ce = std::make_shared<CardioExercise>(state.newEx.name, state.newEx.cardioDuration);
                    state.currentWorkout.addExercise(ce);
                    state.newEx = AddExerciseGUI();
                }
                ImGui::SameLine();
                if (ImGui::Button("Отменить упражнение")) {
                    state.newEx = AddExerciseGUI();
                }
            }
        }
        else if (exType == 2)
        {
            ImGui::Text("Добавление упражнения с собственным весом:");
            ImGui::InputInt("Повторы", &state.newEx.newBwReps);
            if (state.newEx.newBwReps > 0) {
                if (ImGui::Button("Добавить подход")) {
                    state.newEx.bwSets.push_back(state.newEx.newBwReps);
                    state.newEx.newBwReps = 0;
                }
            }
            ImGui::Separator();
            if (!state.newEx.bwSets.empty()) {
                ImGui::Text("Добавленные подходы:");
                int i = 0;
                for (int rep : state.newEx.bwSets) {
                    ImGui::BulletText("%d) %d повторений", ++i, rep);
                }
            }
            ImGui::Separator();
            if (!state.newEx.bwSets.empty()) {
                if (ImGui::Button("Завершить упражнение")) {
                    auto be = std::make_shared<BodyweightExercise>(state.newEx.name);
                    for (int rep : state.newEx.bwSets) {
                        be->addSet(rep);
                    }
                    state.currentWorkout.addExercise(be);
                    state.newEx = AddExerciseGUI();
                }
                ImGui::SameLine();
                if (ImGui::Button("Отменить упражнение")) {
                    state.newEx = AddExerciseGUI();
                }
            }
        }
    }

    ImGui::Separator();
    if (ImGui::Button("Завершить и сохранить тренировку")) {
        state.tracker.addWorkout(state.currentWorkout);
        state.tracker.saveToFile("workouts.txt");
        state.screen = AppScreen::MainMenu;
    }
    ImGui::SameLine();
    if (ImGui::Button("Отменить тренировку")) {
        state.screen = AppScreen::MainMenu;
    }

    ImGui::End();
}

static void ShowAllWorkouts(AppState& state)
{
    ImGui::Begin("Все тренировки");

    if (ImGui::Button("Назад")) {
        state.screen = AppScreen::MainMenu;
    }
    ImGui::Separator();

    int idx = 0;
    for (const auto& w : state.tracker.getAllWorkouts()) {
        char header[64];
        snprintf(header, sizeof(header), "Тренировка %d - %s", ++idx, w.getDate().c_str());
        if (ImGui::TreeNode(header)) {
            for (const auto& ex : w.getExercises()) {
                std::string type = ex->getType();
                std::string name = ex->getName();
                if (type == "Strength") {
                    auto se = std::dynamic_pointer_cast<StrengthExercise>(ex);
                    ImGui::TextColored(ImVec4(0.4f,0.6f,1.0f,1.0f), "Силовое: %s", name.c_str());
                    for (auto& s : se->getSets()) {
                        ImGui::BulletText("%d повторений, %.1f кг", s.reps, s.weight);
                    }
                }
                else if (type == "Cardio") {
                    auto ce = std::dynamic_pointer_cast<CardioExercise>(ex);
                    ImGui::TextColored(ImVec4(0.4f,1.0f,0.4f,1.0f), "Кардио: %s - %d мин", name.c_str(), ce->getDuration());
                }
                else if (type == "Bodyweight") {
                    auto be = std::dynamic_pointer_cast<BodyweightExercise>(ex);
                    ImGui::TextColored(ImVec4(1.0f,0.8f,0.2f,1.0f), "Собств. вес: %s", name.c_str());
                    for (int rep : be->getSets()) {
                        ImGui::BulletText("%d повторений", rep);
                    }
                }
            }
            ImGui::TreePop();
        }
    }

    ImGui::End();
}
static void ShowStatistics(AppState& state)
{
    ImGui::Begin("Статистика");

    if (ImGui::Button("Назад")) {
        state.screen = AppScreen::MainMenu;
    }
    ImGui::Separator();

    std::map<std::string, float> maxWeightPerExercise;
    std::map<std::string, int> maxRepsPerBodyweightExercise;

    ImGui::Text("=== Статистика по тренировкам ===");

    for (const auto& workout : state.tracker.getAllWorkouts()) {
        float totalStrengthWeight = 0.0f;
        int totalCardioTime = 0;
        for (const auto& ex : workout.getExercises()) {
            if (auto se = std::dynamic_pointer_cast<StrengthExercise>(ex)) {
                for (const auto& s : se->getSets()) {
                    totalStrengthWeight += s.reps * s.weight;
                    float& currentMax = maxWeightPerExercise[se->getName()];
                    if (s.weight > currentMax) currentMax = s.weight;
                }
            }
            else if (auto ce = std::dynamic_pointer_cast<CardioExercise>(ex)) {
                totalCardioTime += ce->getDuration();
            }
            else if (auto be = std::dynamic_pointer_cast<BodyweightExercise>(ex)) {
                for (int rep : be->getSets()) {
                    int& currentMax = maxRepsPerBodyweightExercise[be->getName()];
                    if (rep > currentMax) currentMax = rep;
                }
            }
        }
        ImGui::Text("Дата: %s", workout.getDate().c_str());
        ImGui::BulletText("Общий поднятый вес: %.1f кг", totalStrengthWeight);
        ImGui::BulletText("Общее время кардио: %d мин", totalCardioTime);
        ImGui::Separator();
    }

    ImGui::Text("Максимальный вес по силовым упражнениям:");
    for (auto& [name, w] : maxWeightPerExercise) {
        ImGui::BulletText("%s - %.1f кг", name.c_str(), w);
    }

    ImGui::Separator();
    ImGui::Text("Максимальные повторы по упражнениям с собственным весом:");
    for (auto& [name, reps] : maxRepsPerBodyweightExercise) {
        ImGui::BulletText("%s - %d повторений", name.c_str(), reps);
    }

    ImGui::End();
}

int main(int, char**)
{
    if (!glfwInit()) {
        fprintf(stderr, "Ошибка: не удалось инициализировать GLFW\n");
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Трекер тренировок", NULL, NULL);
    if (window == nullptr) {
        fprintf(stderr, "Ошибка: не удалось создать окно GLFW\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Ошибка: не удалось инициализировать GLEW\n");
        return -1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO& io = ImGui::GetIO();
    const char* fontPath = "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf";
    float fontSizePixels = 18.0f;
    ImFont* fontCyr = io.Fonts->AddFontFromFileTTF(
        fontPath,
        fontSizePixels,
        nullptr,
        io.Fonts->GetGlyphRangesCyrillic()
    );
    if (!fontCyr) {
        fprintf(stderr, "Не удалось загрузить шрифт: %s\n", fontPath);
    }

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    AppState state;
    std::ifstream infile("workouts.txt");
    if (infile) {
        state.tracker.loadFromFile("workouts.txt");
    }

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        switch (state.screen)
        {
            case AppScreen::MainMenu:       ShowMainMenu(state);      break;
            case AppScreen::AddWorkout:     ShowAddWorkout(state);    break;
            case AppScreen::ShowAllWorkouts:ShowAllWorkouts(state);   break;
            case AppScreen::Statistics:     ShowStatistics(state);    break;
        }

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.12f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

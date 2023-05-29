/*
    Original design by TheCherno, Walnut
    https://github.com/TheCherno/Walnut
*/

#pragma once

#include <string>


struct GLFWwindow;

struct OverlaySpecification {
    std::string Name = "Vulkan Overlay";
    uint32_t Width = 1200;
    uint32_t Height = 800;
};


class Overlay {
public:
    Overlay(const OverlaySpecification& overlaySpecification = OverlaySpecification());
    ~Overlay();

    void Run();
    void Close();

    virtual void OnUpdate(float ts);

    static Overlay& Get();
    float GetTime();

    GLFWwindow* GetWindowHandle() const { return m_WindowHandle; }


private:
    void Init();
    void Shutdown();


private:
    OverlaySpecification m_Specification;
    GLFWwindow* m_WindowHandle = nullptr;

    bool m_Running = false;

    float m_TimeStep = 0.0f;
    float m_FrameTime = 0.0f;
    float m_LastFrameTime = 0.0f;
};
#pragma once

struct GLFWwindow;

namespace UI
{

	struct ApplicationSpecification
	{
		std::string Name = "ImGui App";
		uint32_t Width = 1600;
		uint32_t Height = 900;
	};

	class Window
	{
	public:
		Window(const ApplicationSpecification& spec);

		void Init();
		void Stop();
		Window& Get();
		void Run();

	private:
		ApplicationSpecification m_Specification;
		GLFWwindow* m_WindowHandle = nullptr;
		bool m_Running = false;

		std::vector<std::shared_ptr<Layer>> m_LayerStack;
	};
};
#pragma once

#include "imgui.h"
#include "Window.h"

#include <string>
#include <functional>

#include "Texture.h"

#include "Camera.h"

class ImGuiLayer
{
public:
	ImGuiLayer();
	~ImGuiLayer();

	static ImGuiLayer* GetLayer()
	{
		if (s_Instance == nullptr)
			s_Instance = new ImGuiLayer();
		return s_Instance;
	}

	/* You have to initialize and free with Init and Shutdown explicitly */
	void Init(Window &window);
	void Shutdown();

	void SetViewportResize(const std::function<void(ImVec2& size)>& resizeCb) { m_ResizeCb = resizeCb; }

	void SetOnCompile(const std::function<void(ImVec2& size)>& compileCb) { m_CompileCb = compileCb; }

	void SetEditorSaveFile(const std::function<bool(const char* location)>& saveCb) { m_SaveCb = saveCb; }
	void SetEditorLoadFile(const std::function<bool(const char* location)>& loadCb) { m_LoadCb = loadCb; }

	void SetFrameTargetCb(const std::function<void(float lastTime, float deltaTime, unsigned int frames, ImVec2 mousePos, bool leftMouse, bool rightMouse)>& frameTarget) { m_FrameTargetCb = frameTarget; }

	void Begin();

	void UpdateViewport(ImTextureID texture_id, const ImVec2& frameSize, Camera& camera, float lastTime, float deltaTime, unsigned int frames);
	void UpdateController(const ImVec2& frameSize, float deltaTime, Texture& tex0, Texture& tex1, Texture& tex2);
	void UpdateEditor(int lineNum, int colNum, int totalLines, bool overWrite, bool canUndo, const char* lang);

	void End();

private:
	std::function<void(ImVec2& size)> m_ResizeCb;
	std::function<void(ImVec2& size)> m_CompileCb;
	std::function<bool(const char* location)> m_LoadCb;
	std::function<bool(const char* location)> m_SaveCb;
	std::function<void(float lastTime, float deltaTime, unsigned int frames, ImVec2 mousePos, bool leftMouse, bool rightMouse)> m_FrameTargetCb;
	std::string m_ErrorString;
	ImVec2 m_ViewportSize;
	bool m_Paused = false;

	static ImGuiLayer* s_Instance;
};

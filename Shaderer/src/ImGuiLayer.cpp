#include "ImGuiLayer.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>

ImGuiLayer* ImGuiLayer::s_Instance = nullptr;

ImGuiLayer::ImGuiLayer()
{
}

ImGuiLayer::~ImGuiLayer()
{
}

void ImGuiLayer::Init(Window& window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    ImGui::StyleColorsDark();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window.m_Window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    io.Fonts->AddFontFromFileTTF("./assets/CascadiaMono.ttf", 16.0f);

}

void ImGuiLayer::Shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiLayer::Begin()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->GetWorkPos());
    ImGui::SetNextWindowSize(viewport->GetWorkSize());
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    static bool dockspaceclosed = false;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Main Dockspace", &dockspaceclosed, window_flags);
    ImGui::PopStyleVar();
    ImGui::PopStyleVar(2);

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockspace_id = ImGui::GetID("Main Dockspace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
}

static bool isInside(const ImVec2& value, const ImVec2& start, const ImVec2& size)
{
    if (value.x >= start.x && value.x <= start.x + size.x)
        if (value.y >= start.y && value.y <= start.y + size.y)
            return true;
    return false;
}

void ImGuiLayer::UpdateViewport(ImTextureID texture_id, const ImVec2& frameSize, Camera& camera, float lastTime, float deltaTime, unsigned int frames)
{
    static bool show_viewport = true;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Shaderer (Hide me after moving)", &show_viewport, ImGuiWindowFlags_NoCollapse);
    m_ViewportSize = ImGui::GetContentRegionAvail();

    if (m_ViewportSize.x != frameSize.x || m_ViewportSize.y != frameSize.y) {
        m_ResizeCb(m_ViewportSize);
    }

    ImGuiIO& io = ImGui::GetIO();

    ImVec2 w_pos = ImGui::GetWindowPos();
    bool passInput = ImGui::IsWindowFocused() && isInside(io.MousePos, w_pos, m_ViewportSize);
    ImVec2 mousePos = passInput ? io.MousePos : ImVec2(0.0, 0.0);
    bool leftMouse = passInput ? io.MouseDown[0] : false;
    bool rightMouse = passInput ? io.MouseDown[1] : false;

    if (passInput) {
        if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_UpArrow))) {
            camera.MoveCamera(MoveDir::Forward, deltaTime);
        } else if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_DownArrow))) {
            camera.MoveCamera(MoveDir::Backward, deltaTime);
        } else if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_RightArrow))) {
            camera.MoveCamera(MoveDir::Right, deltaTime);
        } else if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_LeftArrow))) {
            camera.MoveCamera(MoveDir::Left, deltaTime);
        }
        camera.OnMouseScroll(io.MouseWheel);
        camera.MouseMovement(io.MouseDelta.x, io.MouseDelta.y);
    }

    if (!m_Paused) m_FrameTargetCb(lastTime, deltaTime, frames, mousePos, leftMouse, rightMouse);

    ImGui::Image(texture_id, frameSize, ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();
    ImGui::PopStyleVar();
}

/* Better to be setup and automatically run when updating viewport */
void ImGuiLayer::UpdateController(const ImVec2& frameSize, float deltaTime, Texture& tex0, Texture& tex1, Texture& tex2)
{
    static bool show_controller_window = true;
    static ImVec4 color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);

    ImGuiIO& io = ImGui::GetIO();

    ImGui::Begin("Controller", &show_controller_window);
    if (ImGui::Button("Compile") || (io.KeyCtrl && io.KeysDown[ImGui::GetKeyIndex(ImGuiKey_Enter)]
                && io.KeysDownDuration[ImGui::GetKeyIndex(ImGuiKey_Enter)] <= 0.01)) {
        m_CompileCb(m_ViewportSize);
    }
    ImGui::SameLine();
    if (ImGui::Button("Pause/Play")) {
        m_Paused = m_Paused ? false : true;
    }
    ImGui::SameLine();
    ImGui::Text("Viewport Resolution %2.f x %2.f", frameSize.x, frameSize.y);
    ImGui::SameLine();
    ImGui::Text("Framerate: %0.f FPS", 1.0f / deltaTime);

    ImGui::Separator();
    if (ImGui::ImageButton((void*)(intptr_t)tex0.GetTexId(), ImVec2(32.0f, 32.0f))) {
        ImGui::OpenPopup("change_tex_0");
    }
    ImGui::SameLine();

    if (ImGui::ImageButton((void*)(intptr_t)tex1.GetTexId(), ImVec2(32.0f, 32.0f))) {
        ImGui::OpenPopup("change_tex_1");
    }
    ImGui::SameLine();

    if (ImGui::ImageButton((void*)(intptr_t)tex2.GetTexId(), ImVec2(32.0f, 32.0f))) {
        ImGui::OpenPopup("change_tex_2");
    }

    if (ImGui::BeginPopup("change_tex_0")) {
        static char texloc[256];
        ImGui::InputText("tex_0_location", texloc, IM_ARRAYSIZE(texloc));
        if (ImGui::Button("Load") && strlen(texloc) != 0) {
            if (!tex0.ReloadTexture(texloc)) {
#ifdef DEBUG
                std::cout << "Failed to load texture: " << texloc << std::endl;
#endif
            }
        }
        ImGui::SameLine();
        ImGui::Checkbox("Enabled", &tex0.Enabled);
        ImGui::EndPopup();
    }
    if (ImGui::BeginPopup("change_tex_1")) {
        static char texloc[256];
        ImGui::InputText("tex_1_location", texloc, IM_ARRAYSIZE(texloc));
        if (ImGui::Button("Load") && strlen(texloc) != 0) {
            if (!tex1.ReloadTexture(texloc)) {
#ifdef DEBUG
                std::cout << "Failed to load texture: " << texloc << std::endl;
#endif
            }
        }
        ImGui::SameLine();
        ImGui::Checkbox("Enabled", &tex1.Enabled);
        ImGui::EndPopup();
    }
    if (ImGui::BeginPopup("change_tex_2")) {
        static char texloc[256];
        ImGui::InputText("tex_2_location", texloc, IM_ARRAYSIZE(texloc));
        if (ImGui::Button("Load") && strlen(texloc) != 0) {
            if (!tex2.ReloadTexture(texloc)) {
#ifdef  DEBUG
                std::cout << "Failed to load texture: " << texloc << std::endl;

#endif
            }
        }
        ImGui::SameLine();
        ImGui::Checkbox("Enabled", &tex2.Enabled);
        ImGui::EndPopup();
    }

    ImGui::Text("Builtin color picker");
    ImGui::ColorEdit4("MyColor##2f", (float*)&color, ImGuiColorEditFlags_Float);

    ImGui::End();
}

void ImGuiLayer::UpdateEditor(int lineNum, int colNum, int totalLines, bool overWrite, bool canUndo, const char* lang)
{
    static bool show_editor_window = true;
    static char locationToSave[256];
    ImGui::Begin("Editor", &show_editor_window);
    ImGui::SameLine();
    ImGui::InputText("File Location", locationToSave, IM_ARRAYSIZE(locationToSave));
    ImGui::SameLine();
    if (ImGui::Button("Save") && strlen(locationToSave) != 0) {

        if (!m_SaveCb(locationToSave)) {
            m_ErrorString = "Failed to Write Contents to the file";
            ImGui::OpenPopup("Error");

        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Load") && strlen(locationToSave) != 0) {

        if (!m_LoadCb(locationToSave)) {
            m_ErrorString = "Failed to Read file: ";
            m_ErrorString.append(locationToSave);
            ImGui::OpenPopup("Error");
        }
    }
    ImGui::Separator();
    /*
    ImGui::InputTextMultiline("##shadercode", shaderCode, IM_ARRAYSIZE(shaderCode), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 40), multilineflags);
    */
    ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", lineNum, colNum, totalLines, overWrite ? "Ovr" : "Ins",
        canUndo ? "*" : " ", lang, locationToSave);
}

void ImGuiLayer::End()
{
    /* Error Window */
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal("Error", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text(m_ErrorString.c_str());
        ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 20) / 2);
        ImGui::PushItemWidth(20);
        if (ImGui::Button("OK")) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::SetItemDefaultFocus();
        ImGui::EndPopup();
    }

    /* Editor end!! */
    ImGui::End();

    /* Docking End */
    ImGui::End();

    ImGui::Render();
    //int display_w, display_h;
    //glfwGetFramebufferSize(window.m_Window, &display_w, &display_h);
    //glViewport(0, 0, display_w, display_h);
    //glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    //glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

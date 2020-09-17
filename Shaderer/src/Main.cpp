#include <iostream>
#include "Window.h"

#include "ImGuiLayer.h"

#include "ImGuiColorTextEdit/TextEditor.h"
#include "Editor.h"

#include "GLArrays.h"
#include "Shader.h"
#include "FrameBuffer.h"
#include "Texture.h"

#include "Camera.h"

#include "FileIO.h"

#define WIDTH() 1440
#define HEIGHT() 810

#define FRAMEBUFFER_WIDTH() (WIDTH()-10)
#define FRAMEBUFFER_HEIGHT() (HEIGHT()-10)

#define MAX_EDITOR_SIZE 1024 * 64

/* This option removes the editor, so you can code in your favorite text editor and reload the shader on save */
#define NO_EDITOR_MODE 1

//void clear_error()
//{
//	while (GL_NO_ERROR != glGetError());
//}
//
//void print_any_error()
//{
//    unsigned int err;
//    while ((err = glGetError()) != GL_NO_ERROR) {
//        std::cout << "OpenGL Error " << err << std::endl;
//    }
//}

/*
    More to add here with the full options.
*/
static char shaderCode[MAX_EDITOR_SIZE] =
"#version 460 core \n"
"\n"
"in vec4 gl_FragCoord;\n"
"in vec2 TexCoord;\n"
"\n"
"out vec4 FragColor;\n"
"\n"
"uniform vec2 u_Resolution;\n"
"uniform vec4 u_Mouse;\n"
"uniform float u_Time;\n"
"uniform uint u_Frame;\n"
"uniform float u_DeltaTime;\n"
"uniform vec3 u_CameraPos;\n"
"uniform vec3 u_CameraFront;\n"
"uniform vec3 u_CameraUp;\n"
"uniform vec3 u_CameraRight;\n"
"uniform float u_CameraZoom;\n"
"\n"
"uniform sampler2D texture1;\n"
"uniform sampler2D texture2;\n"
"uniform sampler2D texture3; \n"
"\n"
"/* Only Modify Whatever is below this line */\n"
"\n"
"void main()\n"
"{\n"
"	FragColor = vec4(0.419, 0.196, 0.823, 1.0);\n"
"}\n";
/*
    NOTE: WHENEVER YOU UPDATE UNIFORMS HERE UPDATE IT IN THE SHADER FILE TOO!!!!!
    NOTE: Currently we dont use the enabled flag of texture before update stuff.
*/

/*
    TODO: -> Make a templated abstraction for using ImGui multiline editor or the fully featured TextEditor
    TODO: -> Deprecate u_Mouse access and replace with the camera system.

    Camera System
          -> Add zoom float as uniform (need a permitable range here thats dealt by us) projection matrix done with this.

    BUGS:
          -> Camera Movement is janky rn.
          -> signed distance maybe is the reason we have 2 different shades of white.
*/


static void framebuffer_resize(GLFWwindow* window, int width, int height)
{
    DummyFrameBuffer* main = (DummyFrameBuffer*)glfwGetWindowUserPointer(window);
    if (main == nullptr) return;
    main->SetFrameBufferResize(width, height);
}

/*
// TODO: Do proper deltaTime instead of assuming it always the same.
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (!ImGuiLayer::GetLayer()->ViewPortFocused) return;
    if (action == GLFW_RELEASE) return;

    if (key == GLFW_KEY_W)
        camera.MoveCamera(MoveDir::Forward, 1.0f/60.0f);
    else if (key == GLFW_KEY_S)
        camera.MoveCamera(MoveDir::Backward, 1.0f/60.0f);
    else if (key == GLFW_KEY_D)
        camera.MoveCamera(MoveDir::Right, 1.0f/60.0f);
    else if (key == GLFW_KEY_A)
        camera.MoveCamera(MoveDir::Left, 1.0f/60.0f);
}

static void mouse_scroll_callback(GLFWwindow* window, double xoff, double yoff)
{
    if (!ImGuiLayer::GetLayer()->ViewPortFocused) return;
    camera.OnMouseScroll(yoff);
}

static void mouse_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
    static bool firstMouse = true;
    static float lastX = 0.0f;
    static float lastY = 0.0f;

    if (!ImGuiLayer::GetLayer()->ViewPortFocused) return;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    camera.MouseMovement(lastX - xpos, lastY - ypos);
    lastX = xpos;
    lastY = ypos;
}
*/

int main()
{
    DummyFrameBuffer mainDummy(WIDTH(), HEIGHT(), nullptr);
    Window window(WIDTH(), HEIGHT(), "Shaderer");
    window.SetResizeCb(framebuffer_resize);
    window.SetUserDataPointer(&mainDummy);
    //window.SetMousePosCallback(mouse_pos_callback);
    //window.SetScrollCallback(mouse_scroll_callback);
    //window.SetKeyCallback(key_callback);

    // uncomment to see the perf
    //window.SetSwapInterval(0);

    FrameBuffer shadererFrame(FRAMEBUFFER_WIDTH(), FRAMEBUFFER_HEIGHT(), &mainDummy);
    Camera camera;

    const float squareData[] = {
         1.0f,  1.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
        -1.0f,  1.0f, 0.0f, 1.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
    };

    VertexArray squareVa;
    VertexBuffer squareVb;
    squareVa.Bind();
    squareVb.UploadData(squareData, sizeof(squareData));
    squareVa.SetLayout({
        {ShaderDataType::Float2},
        {ShaderDataType::Float2}
    });

    Shader* mainShader = new Shader("./assets/shaders/simple.vs", "./assets/shaders/simple.fs");
    mainShader->Bind();
    mainShader->UploadUnifrom2f("u_Resolution", FRAMEBUFFER_WIDTH(), FRAMEBUFFER_HEIGHT());

    unsigned int frames = 0;
    static float lastTime = window.GetTime();
    float deltaTime = 1.0/60.0f; // This is just for initialization.
    std::string curErr;

    Texture tex0("./assets/images/temp1.png");
    Texture tex1("./assets/images/temp2.png");
    Texture tex2("./assets/images/temp3.png");

    TextEditor editor;
    auto lang = TextEditor::LanguageDefinition::GLSL();

    /* Setup identifiers here */
    for (size_t i = 0; i < NUMBUILTINFUNCS; i++) {
        TextEditor::Identifier id;
        id.mDeclaration = std::string(BuiltinFuncs[i]);
        lang.mIdentifiers.insert(std::make_pair(std::string(BuiltinFuncs[i]), id));
    }
    for (size_t i = 0; i < NUMVARIABLES; i++)
        lang.mKeywords.insert(std::string(Variables[i]));

    editor.SetLanguageDefinition(lang);

    editor.SetText(shaderCode);

    TextEditor::ErrorMarkers markers;

    ImGuiLayer* imLayer = ImGuiLayer::GetLayer();
    imLayer->Init(window);

    auto resize_cb = [&](ImVec2& size)
    {
#ifdef DEBUG
        std::cout << "Resizing X: " << size.x << " Y: " << size.y << std::endl;
#endif
        shadererFrame.SetFrameBufferResize((unsigned int)size.x, (unsigned int)size.y);
        mainShader->Bind();
        mainShader->UploadUnifrom2f("u_Resolution", size.x, size.y);
    };
    imLayer->SetViewportResize(resize_cb);

    auto recompile_cb = [&](ImVec2& size)
    {
        markers.clear();
        editor.SetErrorMarkers(markers);
        if (!mainShader->UpdateFragmentSrc(editor.GetText())) {
            mainShader->ParseLastError(markers);
            editor.SetErrorMarkers(markers);
        }
        mainShader->Bind();
        mainShader->UploadUnifrom2f("u_Resolution", size.x, size.y);

#ifdef DEBUG
        std::cout << "Recompiling shader with width: "  << size.x << " height: " << size.y << std::endl;
#endif
    };
    imLayer->SetOnCompile(recompile_cb);

    auto save_cb = [&](const char* location)
    {
#ifdef DEBUG
        std::cout << "Saving file: " << location << std::endl;
#endif
        if (!IOManager::GetManager()->WriteToFile(location, editor.GetText().c_str())) {
            return false;
        }
        return true;
    };
    imLayer->SetEditorSaveFile(save_cb);

    auto load_cb = [&](const char* location)
    {
#ifdef DEBUG
        std::cout << "Loading file: " << location << std::endl;
#endif
        std::string fragSrc;
        if (IOManager::GetManager()->ReadFromFile(location, fragSrc)) {
            mainShader->UpdateFragmentSrc(fragSrc);
            editor.SetText(fragSrc);
            mainShader->Bind();
            mainShader->UploadUnifrom2f("u_Resolution", shadererFrame.m_Width, shadererFrame.m_Height);
            return true;
        }
        return false;
    };
    imLayer->SetEditorLoadFile(load_cb);

    auto frametarget_cb = [&](float lastTime, float deltaTime, unsigned int frames, ImVec2 mousePos, bool leftMouse, bool rightMouse)
    {
        ScopedFrameBuffer _scope = shadererFrame.GetScopedFrameBuffer();

        Vec4f mouse(mousePos.x, mousePos.y, leftMouse ? 1.0f : 0.0f, rightMouse ? 1.0f : 0.0f);

        mainShader->Bind();
        mainShader->UploadUniform4f("u_Mouse", mouse);
        mainShader->UploadUniform1f("u_Time", lastTime); // lastTime is good enough
        mainShader->UploadUniform1f("u_DeltaTime", deltaTime);
        mainShader->UploadUniform1ui("u_Frame", frames);
        mainShader->UploadUniform3f("u_CameraPos", camera.Position);
        mainShader->UploadUniform3f("u_CameraFront", camera.Front);
        mainShader->UploadUniform3f("u_CameraUp", camera.Up);
        mainShader->UploadUniform3f("u_CameraRight", camera.Right);
        mainShader->UploadUniform1f("u_CameraZoom", camera.Zoom);

        mainShader->UploadUniform1i("texture1", tex0.GetTexNum());
        tex0.Bind();
        mainShader->UploadUniform1i("texture2", tex1.GetTexNum());
        tex1.Bind();
        mainShader->UploadUniform1i("texture3", tex2.GetTexNum());
        tex2.Bind();

        squareVa.Bind();
        squareVb.Draw(6);
    };

    imLayer->SetFrameTargetCb(frametarget_cb);

    while (window.Running()) {
        window.Poll();

        imLayer->Begin();

        ImVec2 frameSize = ImVec2(shadererFrame.m_Width, shadererFrame.m_Height);
        imLayer->UpdateViewport((void*)(intptr_t)shadererFrame.m_ColorAttachment, frameSize, camera, lastTime, deltaTime, frames);
        imLayer->UpdateController(frameSize, deltaTime, tex0, tex1, tex2);

        auto cpos = editor.GetCursorPosition();
        imLayer->UpdateEditor(cpos.mLine + 1, cpos.mColumn + 1, editor.GetTotalLines(), editor.IsOverwrite(),
            editor.CanUndo(), editor.GetLanguageDefinition().mName.c_str());
        editor.Render("TextEditor");

        imLayer->End();

        window.Update();

        float newTime = window.GetTime();
        deltaTime = newTime - lastTime;
        lastTime = newTime;

        frames++;
    }

    delete mainShader;

    imLayer->Shutdown();

    return 0;
}

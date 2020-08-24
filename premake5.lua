workspace "Shaderer"
    architecture "x86_64"
    startproject "Shaderer"

    configurations { "Debug", "Release" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "3rdparty/GLFW/include"
IncludeDir["Glad"] = "3rdparty/Glad/include"
IncludeDir["ImGui"] = "3rdparty/imgui"

group "Dependencies"
    include "3rdparty/GLFW/"
    include "3rdparty/Glad/"
    include "3rdparty/imgui/"

group ""

project "Shaderer"
    location "Shaderer"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/ImGuiColorTextEdit/TextEditor.h",
        "%{prj.name}/ImGuiColorTextEdit/TextEditor.cpp",

    }

    defines { "GLFW_INCLUDE_NONE" }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui"
    }

    filter "system:windows"
        systemversion "latest"

        links
        {
            "opengl32.lib"
        }

        defines { "PLATFORM_WINDOWS" }

    filter "system:linux"
        systemversion "latest"

        links
        {
            "Xrandr",
            "Xi",
            "X11",
            "dl",
            "pthread"
        }

    filter { "configurations:Debug" }
        defines { "DEBUG" }
        symbols "On"

    filter { "configurations:Release" }
        defines { "NDEBUG" }
        optimize "On"

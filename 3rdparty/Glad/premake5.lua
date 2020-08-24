project "Glad"
    kind "StaticLib"
    language "C"
    staticruntime "on"
    systemversion "latest"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "include/glad/glad.h",
        "include/KHR/khrplatform.h",
        "src/glad.c"
    }

    includedirs
    {
        "include"
    }

    filter "system:linux"
        pic "on"

    filter { "configurations:Debug" }
        defines { "DEBUG" }
        symbols "on"

    filter { "configurations:Release" }
        defines { "NDEBUG" }
        optimize "on"

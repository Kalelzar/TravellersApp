workspace "TravellersApp"
    configurations { "Debug", "Release" }
    architecture "x86_64"

project "TravellersApp"
    location "TravellersApp"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++14"

    targetdir "bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"
    objdir ".obj/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"

    files{
       "%{prj.name}/src/**.hpp",
       "%{prj.name}/src/**.cpp",
    }

    filter "system:windows"
        defines "TRVL_PLATFORM_WINDOWS"
        systemversion "latest"

    filter "system:linux"
        defines "TRVL_PLATFORM_LINUX"
        postbuildcommands "%{cfg.basedir}/%{cfg.buildcfg}.sh"

    filter "configurations:Debug"
        defines "TRVL_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "TRVL_Release"
        optimize "On"


-- premake5.lua

projectDir = "../.."
conanDir = "../conan"

include(conanDir .. "/conanbuildinfo.premake.lua")

linkOptions = conan_exelinkflags

toolSet = "gcc"
if os.host() == "linux" then
    table.insert(linkOptions, "'-lrt'")
    toolSet="gcc"
end

workspace "Template"
    conan_basic_setup()
    architecture "x64"
    configurations { "Debug", "Release" }
    location (projectDir .. "build")
    toolset ("" .. toolSet)

project "SensorNetworkApp"
    location (projectDir .. "build")
    kind "ConsoleApp"
    language "C++"
    targetdir (projectDir .. "bin/%{cfg.buildcfg}")
    cppdialect "C++17"

    --buildoptions("'-Wall'", "'-pedantic'")
    linkoptions(linkOptions)

    files 
    {
        (projectDir .. "/src/**.hpp"), 
        (projectDir .. "/src/**.cpp")
    }
    
    includedirs 
    {
        (projectDir .. "/src/**"),
        (projectDir .. "/src")
    }

    filter "configurations:Debug"
        defines {"DEBUG"}
        symbols "On"

    filter "configurations:Release"
        defines {"NDEBUG"}
        optimize "On"

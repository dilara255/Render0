workspace "Render"
	startproject "Viewer0"

	configurations 
	{
		"Debug",
		"Release",
		"Dist"
	}

	platforms { "x86", "x86_64" }

	filter "platforms:x86"
		architecture "x86"

	filter "platforms:x86_64"
		architecture "x86_64"

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Render0"
	location "Render0"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	dependson {"Aux0"}

	flags
	{
		"MultiProcessorCompile"
	}

	links ("%{wks.location}/Aux0/lib/" .. outputdir .. "/Aux0.lib")

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.h",
		"%{prj.name}/include/**.h"
	}

	includedirs
	{
		"%{wks.location}/Dependencies/spdlog/include",
		"%{prj.name}/include",
		"%{prj.name}/include/RZ_API",
		"%{wks.location}/Aux0/APIs"
	}

	filter "architecture:x86"
		includedirs
		{
			"%{wks.location}/Dependencies/GLFW/include",
		}

		libdirs "%{wks.location}/Dependencies/GLFW/lib-vc2019"
		links "glfw3_mt"
		defines	"X86"


	filter "architecture:x86_64"
		includedirs
		{
			"%{wks.location}/Dependencies/GLFW64/include",
		}

		libdirs "%{wks.location}/Dependencies/GLFW64/lib-vc2019"
		links "glfw3_mt"
		defines "X64"


	filter "system:windows"
		systemversion "latest"
		buildoptions "/MT"

		defines{
			"RZ_PLATFORM_WINDOWS",
			"RZ_BUILD_DLL"
		}

		links { "OpenGL32" }


	filter "configurations:Debug"
		defines "DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines	"RELEASE"
		optimize "on" 		

	filter "configurations:Dist"
		defines "DISTRIB"
		optimize "on" 

	filter {}
	postbuildcommands{
		("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Viewer0")
	}

project "Viewer0"
	location "Viewer0"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	dependson {"Render0"}

	flags
	{
		"MultiProcessorCompile"
	}

	links ("%{wks.location}/Aux0/lib/" .. outputdir .. "/Aux0.lib")

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.h",
		"%{prj.name}/include/**.h"
	}

	includedirs
	{
		"%{wks.location}/Render0/include/RZ_API",
		"%{wks.location}/Dependencies/spdlog/include",
		"%{wks.location}/Aux0/APIs"
	}

	links{
		"Render0"
	}

	filter "system:windows"
		systemversion "latest"
		buildoptions "/MT"

		defines "RZ_PLATFORM_WINDOWS"

	filter "architecture:x86_64"
		defines "X64"

	filter "architecture:x86"
		defines "X86"

	filter "configurations:Debug"
		defines "DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines	"RELEASE"
		optimize "on" 

	filter "configurations:Dist"
		defines	"DISTRIB"
		optimize "on" 

project "Aux0"
	location "Aux0"
	kind "Staticlib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	pchheader "miscStdHeaders.h"
	pchsource "%{prj.name}/src/miscStdHeaders.cpp"

	libdir = ("%{prj.location}/lib/" .. outputdir .. "/")

	flags
	{
		"MultiProcessorCompile"
	}

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.h",
		"%{prj.name}/include/**.h",
		"%{prj.name}/APIs/**.h"
	}

	filter "architecture:x86"
		includedirs
		{
			"%{wks.location}/Dependencies/GLFW/include",
			"%{wks.location}/Dependencies/spdlog/include",
			"%{prj.name}/include",
			"%{prj.name}/APIs"
		}

		libdirs "%{wks.location}/Dependencies/GLFW/lib-vc2019"
		links "glfw3_mt"
		defines	"X86"


	filter "architecture:x86_64"
		includedirs
		{
			"%{wks.location}/Dependencies/GLFW64/include",
			"%{wks.location}/Dependencies/spdlog/include",
			"%{prj.name}/include",
			"%{prj.name}/APIs"
		}

		libdirs "%{wks.location}/Dependencies/GLFW64/lib-vc2019"
		links "glfw3_mt"
		defines "X64"


	filter "system:windows"
		systemversion "latest"
		buildoptions "/MT"

		defines{
			"RZ_PLATFORM_WINDOWS",
			"RZ_BUILD_DLL"
		}

		links { "OpenGL32" }


	filter "configurations:Debug"
		defines "DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines	"RELEASE"
		optimize "on" 		

	filter "configurations:Dist"
		defines "DISTRIB"
		optimize "on" 

	filter {}
	postbuildcommands{
		("{COPY} %{cfg.buildtarget.relpath} " .. libdir)
	}
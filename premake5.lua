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

	IncludeDir = {}
	IncludeDir["GLAD"]    = "%{wks.location}/Dependencies/glad/include"
	IncludeDir["GLFW"]    = "%{wks.location}/Dependencies/GLFW/include"
	IncludeDir["GLFW64"]  = "%{wks.location}/Dependencies/GLFW64/include"
	IncludeDir["GLM"]     = "%{wks.location}/Dependencies/glm/include"
	IncludeDir["SPDLOG"]  = "%{wks.location}/Dependencies/spdlog/include"
	IncludeDir["AUXAPIS"] = "%{wks.location}/Aux0/APIs"
	IncludeDir["RZAPIS"]  = "%{wks.location}/Render0/include/RZ_API"

	LibDir = {}
	LibDir["GLFW"]   = "%{wks.location}/Dependencies/GLFW/lib-vc2019"
	LibDir["GLFW64"] = "%{wks.location}/Dependencies/GLFW64/lib-vc2019"
	LibDir["AUX0"]   = ("%{wks.location}/Aux0/lib/" .. outputdir)
	LibDir["GLAD"]   = ("%{wks.location}/Dependencies/glad/lib/" .. outputdir)

project "Render0"
	location "Render0"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	dependson {"Aux0"}

	defines {"RENDER0", "GLFW_INCLUDE_NONE"}

	flags
	{
		"MultiProcessorCompile"
	}

	links {"%{LibDir.AUX0}/Aux0.lib", "OpenGL32", "%{LibDir.GLAD}/Glad.lib"}

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	shaderdir = "%{prj.location}/src/shaders"

	files
	{
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.h",
		"%{prj.name}/include/**.h",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/include/**.hpp"
	}

	includedirs
	{
		"%{IncludeDir.GLM}",
		"%{wks.location}/Dependencies/spdlog/include",
		"%{prj.name}/include",
		"%{IncludeDir.RZAPIS}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.AUXAPIS}"
	}

	filter "architecture:x86"
		includedirs
		{
			"%{IncludeDir.GLFW}"
		}

		libdirs "%{LibDir.GLFW}"
		links "glfw3_mt"
		defines	"X86"


	filter "architecture:x86_64"
		includedirs
		{
			"%{IncludeDir.GLFW64}"
		}

		libdirs "%{LibDir.GLFW64}"
		links "glfw3_mt"
		defines "X64"


	filter "system:windows"
		systemversion "latest"
		buildoptions "/MT"

		defines{
			"RZ_PLATFORM_WINDOWS",
			"RZ_BUILD_DLL"
		}

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
	postbuildcommands{
		("{COPY} ".. shaderdir .." ../bin/" .. outputdir .. "/Viewer0/shaders")	
	}

project "Viewer0"
	location "Viewer0"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	dependson {"Render0", "Aux0"}

	defines "VIEWER0"

	flags
	{
		"MultiProcessorCompile"
	}

	links ("%{LibDir.AUX0}/Aux0.lib")

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.h",
		"%{prj.name}/include/**.h",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/include/**.hpp"
	}

	includedirs
	{
		"%{IncludeDir.GLM}",	
		"%{IncludeDir.RZAPIS}",
		"%{IncludeDir.SPDLOG}",
		"%{IncludeDir.AUXAPIS}"
	}

	links{
		"Render0"
	}

	filter "system:windows"
		systemversion "latest"
		buildoptions "/MT"

		defines {
			"RZ_PLATFORM_WINDOWS",
			"RZ_BUILD_APP"
		}

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

	defines "AUX0"

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
		"%{prj.name}/APIs/**.h",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/include/**.hpp",
		"%{prj.name}/APIs/**.hpp"
	}

	includedirs {
		"%{IncludeDir.GLM}",
		"%{IncludeDir.SPDLOG}",
		"%{prj.name}/include",
		"%{IncludeDir.AUXAPIS}"
	}

	filter "architecture:x86"
		includedirs
		{
			"%{IncludeDir.GLFW}"
		}

		libdirs "%{LibDir.GLFW}"
		links "glfw3_mt"
		defines	"X86"


	filter "architecture:x86_64"
		includedirs
		{
			"%{IncludeDir.GLFW64}"
		}

		libdirs "%{LibDir.GLFW64}"
		links "glfw3_mt"
		defines "X64"


	filter "system:windows"
		systemversion "latest"
		buildoptions "/MT"

		defines{
			"RZ_PLATFORM_WINDOWS",
			"RZ_BUILD_LIB"
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
		("{COPY} %{cfg.buildtarget.relpath} %{LibDir.AUX0}")
	}
workspace "KingTechnicalTest"
	architecture "x86"
	targetdir "build"
	location "build"

	configurations {
		"Debug",
		"Release"
	}
	startproject "KingTechnicalTest"


outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
project "Cpp-Test"
	location "build/Cpp-Test"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "On"
	
	targetdir ("bin/")
	objdir ("bin-int/")

	defines
	{
		"GLM_FORCE_RADIANS"
	}

	files
	{
		"code/src/**.cpp",
		"code/src/**.c",
		"code/src/**.h",
		"code/src/**.hpp",
		"code/engine/**.cpp",
		"code/engine/**.h"
	}
	
	filter {"system:windows"}
		includedirs 
		{
			"code/external/win/sdl",
			"code/external/common",
			"code/src",
			"code",
			"code/engine"
		}
		
		links
		{
			"opengl32.lib",
			"glew32.lib",
			"glew32s.lib",
			"SDL2.lib",
			"SDL2_image.lib",
			"SDL2main.lib"
		}

        libdirs
        {
            "code/external/win/lib"
        }

		postbuildcommands{ "copy ..\\..\\code\\external\\win\\dll\\* ..\\..\\bin\\ ",
							"xcopy /e /y ..\\..\\code\\assets  .\\assets\\"}
		
		filter {"system:windows","configurations:Release"}
		buildoptions "/MT"
	
	

	filter {"system:macosx"}
	    frameworks_path = path.getabsolute("code/external/osx/frameworks/")

		xcodebuildsettings { ["ARCHS"] = "x86_64" }

		sysincludedirs 
		{
			"code/external/common",
			"code/src",
			"code",
			"code/engine",
            frameworks_path .. "/SDL2.framework/Versions/A/Headers",
			frameworks_path .. "/SDL2_image.framework/Versions/A/Headers"
		}


		links {
		    "OpenGL.framework",
		    "CoreFoundation.framework",
		    "SDL2.framework",
		    "SDL2_image.framework"
		}



		buildoptions {"-F " .. frameworks_path}
		linkoptions {"-F " .. frameworks_path .. " -rpath " .. frameworks_path }

		systemversion "10.12"
		postbuildcommands{ "cp -r " .. path.getabsolute("code/assets") .. " " .. path.getabsolute("bin/") }

	filter {"system:linux"}
		architecture "x86_64"

		sysincludedirs 
		{
			"code/external/common",
			"code/src",
			"code",
			"code/engine"
		}


		buildoptions { 
			"`pkg-config --cflags sdl2`",
			"`pkg-config --cflags SDL2_image`",
			"`pkg-config --cflags glm`"
		}

		linkoptions { 
			"`pkg-config --libs sdl2`",
			"`pkg-config --libs SDL2_image`",
			"`pkg-config --libs glm`",
			"`pkg-config --libs glew`",
			"-lm"
		}
		
		postbuildcommands{ "cp -r " .. path.getabsolute("code/assets") .. " " .. path.getabsolute("bin/") }

	filter {"configurations:Debug"}
		defines
		{
			"KING_DEBUG"
		}
		symbols "On"
		optimize "Off"
		warnings "Extra"

	filter {"configurations:Release"}
		defines
		{
			"KING_RELEASE"
		}
		optimize "Full"
		symbols "Off"
		warnings "Off"

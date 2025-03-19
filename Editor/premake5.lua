project "Oniun Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "Off"
    targetdir(TargetDirectory)
    objdir(ObjectTargetDirectory)
    systemversion "latest"

    debugdir(TargetDirectory)

    files {
        "Source/**.h",
        "Source/**.cpp",
    }

    includedirs {
        "Source",
        "%{wks.location}/Engine/Source",
        "%{wks.location}/Thirdparty/imgui",
        "%{wks.location}/Thirdparty/stb",

        -- Only needed for ImGui when accessing their own files within their API (Don't use this include path use the other)
        "%{wks.location}/Thirdparty/imgui/imgui",
    }

    links {
        "Oniun",
    }

    defines {
        VersionMajor,
        VersionMinor,
        VersionPatch,
    }

    filter "system:windows"
        defines { "ONU_PLATFORM_WINDOWS" }
		filter "action:vs*"
			buildoptions { "/utf-8" }
			defines { "_CRT_SECURE_NO_WARNINGS" }

	filter "system:linux"
	    defines { "ONU_PLATFORM_LINUX" }

	filter "system:macos"
	    defines { "ONU_PLATFORM_MACOS" }

	filter "configurations:Debug"
	    defines { "ONU_DEBUG" }
	    runtime "Debug"
	    symbols "On"

	filter "configurations:Release"
	    defines { "ONU_RELEASE" }
	    runtime "Release"
	    symbols "On"
	    optimize "On"

	filter "configurations:Dist"
	    kind "WindowedApp"
	    defines { "ONU_DIST" }
	    runtime "Release"
	    symbols "Off"
	    optimize "On"


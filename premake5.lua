workspace "Oniun Engine"
    configurations {
        "Debug",
        "Release",
        "Dist",
    }
    architecture "x64"
    startproject "Oniun Editor"

OutputDirectory = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
TargetDirectory = "%{wks.location}/bin/" .. OutputDirectory .. "/%{prj.name}"
ObjectTargetDirectory = "%{wks.location}/bin-int/" .. OutputDirectory .. "/prj.name"

VersionMajor = "ONU_VERSION_MAJOR=0"
VersionMinor = "ONU_VERSION_Minor=0"
VersionPatch = "ONU_VERSION_Patch=0"

include "Thirdparty"
include "Engine"
include "Editor"

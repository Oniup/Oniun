workspace "Oniun Engine"
    configurations {
        "Debug",
        "Release",
        "Dist",
    }
    architecture "x64"
    startproject "Oniun Editor"

OutputDirectory = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
TargetDirectory = "%{wks.location}/Build/" .. OutputDirectory .. "/%{prj.name}"
ObjectTargetDirectory = "%{wks.location}/Build/Obj/" .. OutputDirectory .. "/prj.name"

VersionMajor = "ONU_VERSION_MAJOR=0"
VersionMinor = "ONU_VERSION_MINOR=1"
VersionPatch = "ONU_VERSION_PATCH=0"

include "Engine"
include "Editor"

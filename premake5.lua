include "Premake/extensions.lua"

workspace "Game"
	location "."
	startproject "Launcher" -- proably launcher
	architecture "x64"

	configurations {
		"Debug",
		"Release",
		"Retail"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Source/External"
include "Source/Engine"
include "Source/Game"
include "Source/Launcher"
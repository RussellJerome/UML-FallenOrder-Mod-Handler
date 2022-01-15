#pragma once
#include "Mod/Mod.h"

class FallenOrderModHandler : public Mod
{
public:

	//Basic Mod Info
	FallenOrderModHandler()
	{
		ModName = "Jedi Fallen Order Mod Handler"; // Mod Name -- If Using BP ModActor, Should Be The Same Name As Your Pak
		ModVersion = "1.0.0"; // Mod Version
		ModDescription = "Allows UML to load mods created for FOML"; // Mod Description
		ModAuthors = "RussellJ"; // Mod Author
		ModLoaderVersion = "2.1.0";

		// Dont Touch The Internal Stuff
		ModRef = this;
		CompleteModCreation();
	}

	//Called When Internal Mod Setup is finished
	virtual void InitializeMod() override;

	//Either ProcessInternals or ProcessLocalScriptFunction which you use to communicate between your BPMod and your C++ Mod
	virtual void ProcessFunction(UE4::UObject* obj, UE4::FFrame* Frame) override;

	//InitGameState Call
	virtual void InitGameState() override;

	//Beginplay Hook of Every Actor
	virtual void BeginPlay(UE4::AActor* Actor) override;

	//PostBeginPlay of EVERY Blueprint ModActor
	virtual void PostBeginPlay(std::wstring ModActorName, UE4::AActor* Actor) override;

	virtual void OnModMenuButtonPressed() override;

	//Call ImGui Here (CALLED EVERY FRAME ON DX HOOK)
	virtual void DrawImGui() override;

private:
	// If you have a BP Mod Actor, This is a straight refrence to it
	UE4::AActor* ModActor;
};
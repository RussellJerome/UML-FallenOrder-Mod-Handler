#include "FallenOrderModHandler.h"
#include "Utilities/MinHook.h"


PVOID(*origSay)(void*, UE4::FString*);
PVOID hookSay(void* GM, UE4::FString* Message)
{
	std::wstring WStrMsg = Message->c_str();
	if (WStrMsg.substr(WStrMsg.length() - 6, 6) == L"/Print") // check if message ends with /Print
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		WStrMsg = WStrMsg.substr(0, WStrMsg.length() - 6); // remove /Print extension
		std::string str(WStrMsg.begin(), WStrMsg.end());
		SetConsoleTextAttribute(hConsole, 13);
		std::cout << "Print: " << str << std::endl;
		SetConsoleTextAttribute(hConsole, 7);
	}
	return origSay(GM, Message);
}

// Only Called Once, if you need to hook shit, declare some global non changing values
void FallenOrderModHandler::InitializeMod()
{
	UE4::InitSDK();
	SetupHooks();

	MinHook::Init(); //Uncomment if you plan to do hooks
	MinHook::Add((DWORD64)Pattern::Find("33 C0 4C 8B C2 48 89 44 24 08 33 D2"), &hookSay, &origSay, "AGameMode::Say");
}

bool FirstLoad = true;
UE4::AActor* ModLoaderActor;
void FallenOrderModHandler::InitGameState()
{
	if (FirstLoad)
	{
		UE4::FTransform transform;
		transform.Translation = UE4::FVector(0, 0, 0);
		transform.Rotation = UE4::FQuat(0, 0, 0, 0);
		transform.Scale3D = UE4::FVector(1, 1, 1);
		UE4::FActorSpawnParameters spawnParams = UE4::FActorSpawnParameters::FActorSpawnParameters();
		UE4::UClass* ModActorObject = UE4::UClass::LoadClassFromString(L"/Game/ModLoaderContent/ModLoaderActor.ModLoaderActor_C", false);
		if (ModActorObject)
		{
			ModActor = UE4::UWorld::GetWorld()->SpawnActor(ModActorObject, &transform, &spawnParams);
			if (ModActor)
			{
				Log::Info("Sucessfully Loaded ModLoader Pak");
				ModLoaderActor = ModActor;
			}

		}
		FirstLoad = false;
	}
	if (ModLoaderActor)
	{
		ModLoaderActor->CallFunctionByNameWithArguments(L"CleanLoader", nullptr, NULL, true);
	}
}

void FallenOrderModHandler::BeginPlay(UE4::AActor* Actor)
{
	if (Actor->IsA(UE4::ACustomClass::StaticClass(GameProfile::SelectedGameProfile.BeginPlayOverwrite)))
	{
		if (ModLoaderActor)
		{
			ModLoaderActor->CallFunctionByNameWithArguments(L"PostLoaderStart", nullptr, NULL, true);
		}
	}
}

void FallenOrderModHandler::PostBeginPlay(std::wstring ModActorName, UE4::AActor* Actor)
{
	// Filters Out All Mod Actors Not Related To Your Mod
	std::wstring TmpModName(ModName.begin(), ModName.end());
	if (ModActorName == TmpModName)
	{
		//Sets ModActor Ref
		ModActor = Actor;
	}
}

void FallenOrderModHandler::DX11Present(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, ID3D11RenderTargetView* pRenderTargetView)
{
}

void FallenOrderModHandler::OnModMenuButtonPressed()
{
}

void FallenOrderModHandler::DrawImGui()
{
}
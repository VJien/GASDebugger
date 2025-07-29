// Copyright VJien, Inc. All Rights Reserved.

#include "GASDebuggerLogger.h"
#include "HAL/FileManager.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"
#include "GASDebugger/GASDebuggerSettings.h"

TMap<ELogCategory, TUniquePtr<FArchive>> FGASDebuggerLogger::LogArchives;
FString FGASDebuggerLogger::SessionDir;
bool FGASDebuggerLogger::bIsLogging = false;

void FGASDebuggerLogger::StartLogging()
{
	if (!UGASDebuggerSettings::Get()->bEnableLogging)
	{
		return;
	}

	bIsLogging = true;
	SessionDir = FPaths::ProjectLogDir() / TEXT("GASDebugger") / FDateTime::Now().ToString();
	IFileManager::Get().MakeDirectory(*SessionDir, true);
}

void FGASDebuggerLogger::StopLogging()
{
	if (!bIsLogging)
	{
		return;
	}

	for (auto& Pair : LogArchives)
	{
		if (Pair.Value)
		{
			Pair.Value->Close();
		}
	}
	LogArchives.Empty();
	bIsLogging = false;
}

void FGASDebuggerLogger::Log(ELogCategory Category, const FString& Message)
{
	if (!bIsLogging)
	{
		return;
	}

	if (!LogArchives.Contains(Category))
	{
		FString CategoryName;
		switch (Category)
		{
		case ELogCategory::Abilities:
			CategoryName = TEXT("Abilities");
			break;
		case ELogCategory::Tags:
			CategoryName = TEXT("Tags");
			break;
		case ELogCategory::Attributes:
			CategoryName = TEXT("Attributes");
			break;
		case ELogCategory::GameplayEffects:
			CategoryName = TEXT("GameplayEffects");
			break;
		default:
			return; 
		}

		FString FilePath = SessionDir / (CategoryName + TEXT(".log"));
		FArchive* Ar = IFileManager::Get().CreateFileWriter(*FilePath, FILEWRITE_Append);
		if (Ar)
		{
			LogArchives.Add(Category, TUniquePtr<FArchive>(Ar));
		}
		else
		{
			return; // Failed to create file writer
		}
	}

	if (FArchive* Ar = LogArchives[Category].Get())
	{
		FString LogLine = FString::Printf(TEXT("%s\r\n"), *Message);
		Ar->Serialize(TCHAR_TO_ANSI(*LogLine), LogLine.Len());
		Ar->Flush();
	}
}

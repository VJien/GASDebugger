// Copyright VJien, Inc. All Rights Reserved.

#include "GASDebuggerLogger.h"
#include "HAL/FileManager.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"
#include "GASDebugger/GASDebuggerSettings.h"

TMap<ELogCategory, TUniquePtr<FArchive>> FGASDebuggerLogger::LogArchives;
FString FGASDebuggerLogger::FileDire = FPaths::ConvertRelativePathToFull(FPaths::ProjectLogDir())  / TEXT("GASDebugger");
FString FGASDebuggerLogger::SessionDir;
bool FGASDebuggerLogger::bIsLogging = false;

FGASDebuggerLogger::FGASDebuggerLogger()
{
}

void FGASDebuggerLogger::StartLogging()
{
	if (!UGASDebuggerSettings::Get()->bEnableLogging)
	{
		return;
	}
	SessionDir = FileDire / FDateTime::Now().ToString();
	bIsLogging = true;
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

    auto GetOrCreateLogArchive = [&](ELogCategory LogCategory) -> FArchive*
    {
        if (!LogArchives.Contains(LogCategory))
        {
            FString CategoryName;
            switch (LogCategory)
            {
            case ELogCategory::Abilities: CategoryName = TEXT("Abilities"); break;
            case ELogCategory::Tags: CategoryName = TEXT("Tags"); break;
            case ELogCategory::Attributes: CategoryName = TEXT("Attributes"); break;
            case ELogCategory::GameplayEffects: CategoryName = TEXT("GameplayEffects"); break;
            case ELogCategory::All: CategoryName = TEXT("All"); break;
            default: return nullptr;
            }

            FString FilePath = SessionDir / (CategoryName + TEXT(".log"));
            FArchive* Ar = IFileManager::Get().CreateFileWriter(*FilePath, FILEWRITE_Append);
            if (Ar)
            {
                LogArchives.Add(LogCategory, TUniquePtr<FArchive>(Ar));
            }
            return Ar;
        }
        return LogArchives[LogCategory].Get();
    };

    // Log to the specific category file
    if (FArchive* CategoryAr = GetOrCreateLogArchive(Category))
    {
        FString LogLine = FString::Printf(TEXT("%s\r\n"), *Message);
        CategoryAr->Serialize(TCHAR_TO_ANSI(*LogLine), LogLine.Len());
        CategoryAr->Flush();
    }

    // Also log to the "All" file
    if (FArchive* AllAr = GetOrCreateLogArchive(ELogCategory::All))
    {
        FString LogLine = FString::Printf(TEXT("%s\r\n"), *Message);
        AllAr->Serialize(TCHAR_TO_ANSI(*LogLine), LogLine.Len());
        AllAr->Flush();
    }
}

FString FGASDebuggerLogger::GetLogFilePath()
{
	return FileDire;
}

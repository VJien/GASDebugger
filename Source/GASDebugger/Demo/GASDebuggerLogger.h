// Copyright VJien, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

enum class ELogCategory : uint8
{
	Abilities,
	Tags,
	Attributes,
	GameplayEffects,
	All
};

class FGASDebuggerLogger
{
public:
	FGASDebuggerLogger();
	
	static void StartLogging();
	static void StopLogging();
	static void Log(ELogCategory Category, const FString& Message);

	static FString GetLogFilePath();
private:
	static TMap<ELogCategory, TUniquePtr<class FArchive>> LogArchives;
	static FString SessionDir;
	static FString FileDire;
	static bool bIsLogging;
};

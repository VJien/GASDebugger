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
	static void StartLogging();
	static void StopLogging();
	static void Log(ELogCategory Category, const FString& Message);

private:
	static TMap<ELogCategory, TUniquePtr<class FArchive>> LogArchives;
	static FString SessionDir;
	static bool bIsLogging;
};

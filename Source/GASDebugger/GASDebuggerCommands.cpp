// Copyright Epic Games, Inc. All Rights Reserved.

#include "GASDebuggerCommands.h"

#define LOCTEXT_NAMESPACE "FGASDebuggerModule"

void FGASDebuggerCommands::RegisterCommands()
{
	UI_COMMAND(GASDebuggerCommand, "GASDebugger", "Bring up GASDebugger window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE

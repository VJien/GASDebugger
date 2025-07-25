// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "GASDebuggerStyle.h"

class FGASDebuggerCommands : public TCommands<FGASDebuggerCommands>
{
public:

	FGASDebuggerCommands()
		: TCommands<FGASDebuggerCommands>(TEXT("GASDebugger"), NSLOCTEXT("Contexts", "GASDebugger", "GASDebugger Plugin"), NAME_None, FGASDebuggerStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};
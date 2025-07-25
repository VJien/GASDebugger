// Copyright Epic Games, Inc. All Rights Reserved.

#include "GASDebuggerModule.h"
#include "GASDebuggerStyle.h"
#include "GASDebuggerCommands.h"
#include "GASDebuggerSettings.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "Blueprint/UserWidget.h"

static const FName GASDebuggerTabName("GASDebugger");

#define LOCTEXT_NAMESPACE "FGASDebuggerModule"

void FGASDebuggerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FGASDebuggerStyle::Initialize();
	FGASDebuggerStyle::ReloadTextures();

	FGASDebuggerCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FGASDebuggerCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FGASDebuggerModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FGASDebuggerModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(GASDebuggerTabName, FOnSpawnTab::CreateRaw(this, &FGASDebuggerModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FGASDebuggerTabTitle", "GASDebugger"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FGASDebuggerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FGASDebuggerStyle::Shutdown();

	FGASDebuggerCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(GASDebuggerTabName);
}

TSharedRef<SDockTab> FGASDebuggerModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	auto&& WidgetClass =  UGASDebuggerSettings::Get()->Widget.LoadSynchronous();
	UUserWidget* EdUI = nullptr;
	if (WidgetClass)
	{
		UWorld* WidgetWorld = GEditor->GetEditorWorldContext().World();
		if (WidgetWorld)
		{
			EdUI = CreateWidget<UUserWidget>(WidgetWorld, WidgetClass);
		}
	}    if (EdUI)
	{
		
		return SNew(SDockTab)
			.TabRole(ETabRole::NomadTab)
			[
				// Put your tab content here!
				SNew(SBox)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					EdUI->TakeWidget()
				]
			];
	}
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock).Text(LOCTEXT("GASDebuggerTabContent", "GAS Debugger is not configured. Please set the Widget in the plugin settings."))
			]
		];
}


void FGASDebuggerModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(GASDebuggerTabName);
}

void FGASDebuggerModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FGASDebuggerCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FGASDebuggerCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGASDebuggerModule, GASDebugger)
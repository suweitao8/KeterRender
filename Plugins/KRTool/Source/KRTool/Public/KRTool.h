// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

//����һ���ಥ�����㲻ͬ�Ķ���󶨵��ô�����
DECLARE_MULTICAST_DELEGATE(FOnActionbutClick);


class FKRToolModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:

#pragma region ContentBrowserMenuExtention

	TArray<FString> SelectedFolderPaths;

	void InitCBMenuExtension();

	TSharedRef<FExtender> CustomCBMenuExtender(const TArray<FString>& SelectedPaths);

	void AddCBMenuEntry(class FMenuBuilder& MenuBuilder);

	void OnDeleteUnusedAssetButtonClicked();

	void OnDeleteEmptyFolderButtonClicked();

	void FixUpRedirectors();

#pragma endregion

};

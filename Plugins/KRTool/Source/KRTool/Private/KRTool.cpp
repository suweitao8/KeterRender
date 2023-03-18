// Copyright Epic Games, Inc. All Rights Reserved.

#include "DebugHeader.h"
#include "ContentBrowserModule.h"
#include "ContentBrowser/Public/ContentBrowserModule.h"
#include "ContentBrowser/Public/ContentBrowserDelegates.h" 
#include "EditorAssetLibrary.h"
#include "AssetRegistryModule.h"
#include "ObjectTools.h"
#include "AssetToolsModule.h"
#include "KRTool.h"

#define LOCTEXT_NAMESPACE "FKRToolModule"

void FKRToolModule::StartupModule()
{
	InitCBMenuExtension();
}

void FKRToolModule::ShutdownModule()
{
}

void FKRToolModule::InitCBMenuExtension()
{
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
	TArray<FContentBrowserMenuExtender_SelectedPaths>& MenuExtenderDelegates = ContentBrowserModule.GetAllPathViewContextMenuExtenders();
	MenuExtenderDelegates.Add(FContentBrowserMenuExtender_SelectedPaths::CreateRaw(this, &FKRToolModule::CustomCBMenuExtender));
}

TSharedRef<FExtender> FKRToolModule::CustomCBMenuExtender(const TArray<FString>& SelectedPaths)
{
	TSharedRef<FExtender> MenuExtender(new FExtender());

	if (SelectedPaths.Num() > 0)
	{
		SelectedFolderPaths = SelectedPaths;

		MenuExtender->AddMenuExtension(FName("Delete"),
			EExtensionHook::After,
			nullptr,
			FMenuExtensionDelegate::CreateRaw(this, &FKRToolModule::AddCBMenuEntry));
	}

	return MenuExtender;
}

void FKRToolModule::AddCBMenuEntry(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.AddMenuEntry(FText::FromString(TEXT("Delete Unused Assets")),
		FText::FromString(TEXT("Safely delete all unused assets under folder.")),
		FSlateIcon(),
		FExecuteAction::CreateRaw(this, &FKRToolModule::OnDeleteUnusedAssetButtonClicked));

	MenuBuilder.AddMenuEntry(FText::FromString(TEXT("Delete Empty Folders")),
		FText::FromString(TEXT("Safely delete all empty folder.")),
		FSlateIcon(),
		FExecuteAction::CreateRaw(this, &FKRToolModule::OnDeleteEmptyFolderButtonClicked));
}

void FKRToolModule::OnDeleteUnusedAssetButtonClicked()
{
	if (SelectedFolderPaths.Num() > 1)
	{
		DebugHeader::ShowMessage(EAppMsgType::Ok, TEXT("You can only do this to one folder."));
		return;
	}

	TArray<FString> AssetPaths = UEditorAssetLibrary::ListAssets(SelectedFolderPaths[0]);
	if (AssetPaths.Num() == 0)
	{
		DebugHeader::ShowMessage(EAppMsgType::Ok, TEXT("No asset found under selected folder."));
		return;
	}

	// 修复重定向
	FixUpRedirectors();

	TArray<FAssetData> UnusedAssetDatas;
	for (const FString& AssetPath : AssetPaths)
	{
		// Dont't touch root folder
		if (AssetPath.Contains(TEXT("Developers")) ||
			AssetPath.Contains(TEXT("Collections")) ||
			AssetPath.Contains(TEXT("__External"))) continue;
		if (UEditorAssetLibrary::DoesAssetExist(AssetPath) == false) continue;

		TArray<FString> AssetReferences = UEditorAssetLibrary::FindPackageReferencersForAsset(AssetPath);
		if (AssetReferences.Num() == 0)
		{
			const FAssetData UnusedAssetData = UEditorAssetLibrary::FindAssetData(AssetPath);
			UnusedAssetDatas.Add(UnusedAssetData);
		}
	}

	if (UnusedAssetDatas.Num() > 0)
	{
		ObjectTools::DeleteAssets(UnusedAssetDatas, true);
	}
	else
	{
		DebugHeader::ShowMessage(EAppMsgType::Ok, TEXT("No unused asset found under selected folder."));
	}
}

void FKRToolModule::OnDeleteEmptyFolderButtonClicked()
{
	FixUpRedirectors();

	TArray<FString> FolderPathArray = UEditorAssetLibrary::ListAssets(SelectedFolderPaths[0], true, true);
	uint32 Counter = 0;

	FString EmptyFolderNames;
	TArray<FString> EmptyFolderPathArray;

	for (const FString& FolderPath : FolderPathArray)
	{
		if (FolderPath.Contains(TEXT("Developers")) ||
			FolderPath.Contains(TEXT("Collections")) ||
			FolderPath.Contains(TEXT("__External"))) continue;

		if (UEditorAssetLibrary::DoesDirectoryExist(FolderPath) == false) continue;
		if (UEditorAssetLibrary::DoesDirectoryHaveAssets(FolderPath) == false)
		{
			EmptyFolderNames.Append(FolderPath);
			EmptyFolderNames.Append(TEXT("\n"));

			EmptyFolderPathArray.Add(FolderPath);
		}
	}

	if (EmptyFolderPathArray.Num() == 0)
	{
		DebugHeader::ShowMessage(EAppMsgType::Ok, TEXT("No empty folder found under selected folder"), false);
		return;
	}

	// 弹出确认窗口
	EAppReturnType::Type ConfirmResult = DebugHeader::ShowMessage(EAppMsgType::OkCancel, TEXT("Empty folders found in:\n") + 
		EmptyFolderNames + TEXT("\nWould you like to delete all?"), false);
	if (ConfirmResult == EAppReturnType::Cancel) return;

	for (const FString& FolderPath : EmptyFolderPathArray)
	{
		if (UEditorAssetLibrary::DeleteDirectory(FolderPath))
		{
			++Counter;
		}
	}

	if (Counter > 0)
	{
		DebugHeader::ShowNotifyInfo(TEXT("删除空文件夹成功!"));
	}
}

void FKRToolModule::FixUpRedirectors()
{
	TArray<UObjectRedirector*> Redirectors;

	// 获取到 FAssetRegistryModule
	FAssetRegistryModule& AssetRegistryModule =
		FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));

	// 创建过滤器
	FARFilter Filter;
	Filter.bRecursivePaths = true;
	Filter.PackagePaths.Emplace("/Game");
	Filter.ClassNames.Emplace("ObjectRedirector");

	// 使用过滤器得到所有需要重定向的资产
	TArray<FAssetData> OutRedirectorAssetDatas;
	AssetRegistryModule.Get().GetAssets(Filter, OutRedirectorAssetDatas);

	// 写入到 Redirectors 里面
	for (const FAssetData RedirectorData : OutRedirectorAssetDatas)
	{
		if (UObjectRedirector* Redirector = Cast<UObjectRedirector>(RedirectorData.GetAsset()))
		{
			Redirectors.Add(Redirector);
		}
	}

	// 获取到 FAssetToolsModule
	FAssetToolsModule& AssetToolsModule =
		FModuleManager::LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools"));

	// 调用重定向
	AssetToolsModule.Get().FixupReferencers(Redirectors);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FKRToolModule, KRTool)











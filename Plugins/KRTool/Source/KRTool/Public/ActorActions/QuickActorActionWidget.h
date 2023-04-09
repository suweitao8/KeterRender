// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "QuickActorActionWidget.generated.h"

UENUM(BlueprintType)
enum class E_DuplicationAxis : uint8
{
	UDA_XAxis UMETA(DisplayName = "X Axis"),
	UDA_YAxis UMETA(DisplayName = "Y Axis"),
	UDA_ZAxis UMETA(DisplayName = "Z Axis"),
	UDA_Max UMETA(DisplayName = "Default Max"),
};

USTRUCT(BlueprintType)
struct FRandomActorRotation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "bRandomizeRotYaw")
		bool bRandomizeRotYaw = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RotYawMin", meta = (EditCondition = "bRandomizeRotYaw"))
		float RotYawMin = -45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RotYawMax", meta = (EditCondition = "bRandomizeRotYaw"))
		float RotYawMax = 45.0f;
};

UCLASS()
class KRTOOL_API UQuickActorActionWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SearchCase")
		TEnumAsByte<ESearchCase::Type> SearchCase = ESearchCase::CaseSensitive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NumberOfDuplicate")
		int32 NumberOfDuplicate = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Offset")
		float Offset = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AxisForDuplication")
		E_DuplicationAxis AxisForDuplication = E_DuplicationAxis::UDA_XAxis;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomActorRotation")
		FRandomActorRotation RandomActorRotation;

	UFUNCTION(BlueprintCallable, Category = "SelectAllActorsWithSimilarName")
		void SelectAllActorsWithSimilarName();

	UFUNCTION(BlueprintCallable, Category = "DuplicateActor")
		void DuplicateActor();

	UFUNCTION(BlueprintCallable, Category = "RandomizeActorTransform")
		void RandomizeActorTransform();

private:
	UPROPERTY()
		class UEditorActorSubsystem* EditorActorSubsystem;

	bool GetEditorActorSubsystem();
};

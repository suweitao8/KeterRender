// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorActions/QuickActorActionWidget.h"
#include "Subsystems/EditorActorSubsystem.h"
#include "DebugHeader.h"

void UQuickActorActionWidget::SelectAllActorsWithSimilarName()
{
	if (GetEditorActorSubsystem() == false) return;

	TArray<AActor*> SelectedActors = EditorActorSubsystem->GetSelectedLevelActors();
	if (SelectedActors.Num() == 0)
	{
		DebugHeader::ShowNotifyInfo(TEXT("You must select one actor."));
		return;
	}

	FString SelectedActorName = SelectedActors[0]->GetActorLabel();
	const FString NameToSearch = SelectedActorName.LeftChop(4);

	TArray<AActor*> AllLevelActors = EditorActorSubsystem->GetAllLevelActors();
	for (AActor* Actor : AllLevelActors)
	{
		if (!Actor) continue;

		if (Actor->GetActorLabel().Contains(NameToSearch, SearchCase))
		{
			EditorActorSubsystem->SetActorSelectionState(Actor, true);
		}
	}
}

void UQuickActorActionWidget::DuplicateActor()
{
	if (GetEditorActorSubsystem() == false) return;

	TArray<AActor*> SelectedActors = EditorActorSubsystem->GetSelectedLevelActors();
	if (SelectedActors.Num() == 0)
	{
		DebugHeader::ShowNotifyInfo(TEXT("没有选中角色"));
		return;
	}

	if (NumberOfDuplicate <= 0 || Offset == 0)
	{
		DebugHeader::ShowNotifyInfo(TEXT("没有指定复制数量或者偏移"));
		return;
	}

	for (AActor* SelectedActor : SelectedActors)
	{
		if (!SelectedActor) continue;

		for (int32 i = 0; i < NumberOfDuplicate; i++)
		{
			AActor* DuplicatedActor = EditorActorSubsystem->DuplicateActor(SelectedActor, SelectedActor->GetWorld());
			if (!DuplicatedActor) continue;

			const float DuplicationOffsetDistance = (i + 1) * Offset;
			switch (AxisForDuplication)
			{
			case E_DuplicationAxis::UDA_XAxis:
				DuplicatedActor->AddActorWorldOffset(FVector(DuplicationOffsetDistance, 0.0f, 0.0f));
				break;
			case E_DuplicationAxis::UDA_YAxis:
				DuplicatedActor->AddActorWorldOffset(FVector(0.0f, DuplicationOffsetDistance, 0.0f));
				break;
			case E_DuplicationAxis::UDA_ZAxis:
				DuplicatedActor->AddActorWorldOffset(FVector(0.0f, 0.0f, DuplicationOffsetDistance));
				break;
			case E_DuplicationAxis::UDA_Max:
				break;
			default:
				break;
			}

			EditorActorSubsystem->SetActorSelectionState(DuplicatedActor, true);
		}
	}
}

void UQuickActorActionWidget::RandomizeActorTransform()
{
	if (GetEditorActorSubsystem() == false) return;

	if (!RandomActorRotation.bRandomizeRotYaw)
	{
		DebugHeader::ShowNotifyInfo(TEXT("Dont handle actor rotation."));
		return;
	}

	TArray<AActor*> SelectedActors = EditorActorSubsystem->GetSelectedLevelActors();
	if (SelectedActors.Num() == 0)
	{
		DebugHeader::ShowNotifyInfo(TEXT("No actor selected."));
		return;
	}

	for (AActor* SelectedActor : SelectedActors)
	{
		if (!SelectedActor) continue;

		const float RandomRotYawValue = FMath::RandRange(RandomActorRotation.RotYawMin, RandomActorRotation.RotYawMax);
		SelectedActor->AddActorWorldRotation(FRotator(0.0f, RandomRotYawValue, 0.0f));
	}
}

bool UQuickActorActionWidget::GetEditorActorSubsystem()
{
	if (!EditorActorSubsystem)
	{
		EditorActorSubsystem = GEditor->GetEditorSubsystem<UEditorActorSubsystem>();
	}
	return EditorActorSubsystem != nullptr;
}

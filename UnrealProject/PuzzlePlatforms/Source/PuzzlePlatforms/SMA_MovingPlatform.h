// Troy Records Jr.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "SMA_MovingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API ASMA_MovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	ASMA_MovingPlatform();

	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;

	FORCEINLINE void AddActiveTrigger()  { ActiveTriggers++; }
	FORCEINLINE void RemoveActiveTrigger() { ActiveTriggers--; }

private:
	UPROPERTY(EditAnywhere, Category = "Setup", meta = (AllowPrivateAccess = "true"))
	float MovementSpeed = 5.0f;
	
	UPROPERTY(EditAnywhere, Category = "Setup", meta = (AllowPrivateAccess = "true", MakeEditWidget = "true"))
	FVector TargetLocation;

	FVector GlobalTargetLocation;
	FVector GlobalStartLocation;

	UPROPERTY(EditAnywhere, Category = "Setup", meta = (AllowPrivateAccess = "true"))
	int ActiveTriggers = 1;
	
	void Move(float DeltaSeconds);
	
};

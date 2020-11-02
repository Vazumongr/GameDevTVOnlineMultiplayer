// Troy Records Jr.


#include "SMA_MovingPlatform.h"

ASMA_MovingPlatform::ASMA_MovingPlatform()
{
    PrimaryActorTick.bCanEverTick = true;

    SetMobility(EComponentMobility::Movable);
}

void ASMA_MovingPlatform::BeginPlay()
{
    Super::BeginPlay();

    if(HasAuthority())    // Are we the server-client? (We are the one hosting the server)
    {
        SetReplicates(true);    // Means this is calculated on the server then send info to clients
        SetReplicateMovement(true); // Means this actors location is calculated on the server then send info to clients
    }

    GlobalStartLocation = GetActorLocation();
    GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);
}

void ASMA_MovingPlatform::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if(HasAuthority())
    {
        if(ActiveTriggers > 0)
            Move(DeltaSeconds);
    }
}

void ASMA_MovingPlatform::Move(float DeltaSeconds)
{
    FVector Location = GetActorLocation();
        
    FVector Direction = (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal();
    float DistanceToTarget = FVector::Dist(GlobalTargetLocation, GlobalStartLocation);
    float DistanceFromStart = FVector::Dist(Location, GlobalStartLocation);

    if(DistanceFromStart > DistanceToTarget)
    {
        FVector temp = GlobalTargetLocation;
        GlobalTargetLocation = GlobalStartLocation;
        GlobalStartLocation = temp;
    }
    
    Location += Direction * DeltaSeconds * MovementSpeed;
    SetActorLocation(Location);
}

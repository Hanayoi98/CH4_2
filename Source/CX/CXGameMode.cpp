#include "CXGameMode.h"
#include "CXPlayerState.h"
#include "CXPlayerController.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "GameFramework/PlayerStart.h"
#include "CXGameState.h"
#include "TimerManager.h"

ACXGameMode::ACXGameMode()
{
	PlayerControllerClass = ACXPlayerController::StaticClass();
	PlayerStateClass = ACXPlayerState::StaticClass();
	GameStateClass = ACXGameState::StaticClass();
}

void ACXGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (ACXGameState* CXGameState = GetGameState<ACXGameState>())
	{
		CXGameState->SetCurrentState(ECXGameState::Waiting);
		CXGameState->SetRemainingTime(WaitingTime);
	}
}

void ACXGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (bCountdownStarted)
	{
		return;
	}

	bCountdownStarted = true;
	GetWorldTimerManager().SetTimer(CountdownTimer, this, &ThisClass::TickCountdown, 1.0f, true);
}

void ACXGameMode::TickCountdown()
{
	ACXGameState* CXGameState = GetGameState<ACXGameState>();
	if (IsValid(CXGameState) == false)
	{
		return;
	}

	const int32 Remaining = FMath::Max(CXGameState->GetRemainingTime() - 1, 0);
	CXGameState->SetRemainingTime(Remaining);

	if (Remaining > 0)
	{
		return;
	}

	if (CXGameState->GetCurrentState() == ECXGameState::Waiting)
	{
		CXGameState->SetCurrentState(ECXGameState::Playing);
		CXGameState->SetRemainingTime(PlayTime);
	}
	else if (CXGameState->GetCurrentState() == ECXGameState::Playing)
	{
		FinishGame(CXGameState);
	}
}

void ACXGameMode::FinishGame(ACXGameState* CXGameState)
{
	GetWorldTimerManager().ClearTimer(CountdownTimer);

	int32 MaxCoins = -1;
	TArray<APlayerState*> Winners;

	for (APlayerState* PlayerState : CXGameState->PlayerArray)
	{
		const ACXPlayerState* CXPlayerState = Cast<ACXPlayerState>(PlayerState);
		if (IsValid(CXPlayerState) == false)
		{
			continue;
		}

		if (CXPlayerState->GetCoins() > MaxCoins)
		{
			MaxCoins = CXPlayerState->GetCoins();
			Winners.Reset();
			Winners.Add(PlayerState);
		}
		else if (CXPlayerState->GetCoins() == MaxCoins)
		{
			Winners.Add(PlayerState);
		}
	}

	CXGameState->SetWinners(Winners);
	CXGameState->SetCurrentState(ECXGameState::Finished);
}

AActor* ACXGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<APlayerStart*> AvailablePlayerStarts;

	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		APlayerStart* PlayerStart = *It;

		if (IsPlayerStartAvailable(PlayerStart))
		{
			AvailablePlayerStarts.Add(PlayerStart);
		}
	}

	checkf(AvailablePlayerStarts.IsEmpty() == false, TEXT("There is no available Vehicle PlayerStart."));

	const int32 RandomIndex = FMath::RandHelper(AvailablePlayerStarts.Num());
	return Cast<AActor>(AvailablePlayerStarts[RandomIndex]);
}

bool ACXGameMode::IsPlayerStartAvailable(const APlayerStart* PlayerStart) const
{
	if (IsValid(PlayerStart) == false || IsValid(GetWorld()) == false)
	{
		return false;
	}

	const FTransform PlayerStartTransform = PlayerStart->GetActorTransform();
	const FVector CheckLocation = PlayerStartTransform.TransformPosition(VehicleSpawnCheckOffset);
	const FQuat CheckRotation = PlayerStartTransform.GetRotation();
	const FCollisionShape CheckShape = FCollisionShape::MakeBox(VehicleSpawnCheckHalfExtent);
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Vehicle);

	const FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(VehiclePlayerStartCheck), false);

	const bool bVehicleExists =
		GetWorld()->OverlapAnyTestByObjectType(
			CheckLocation,
			CheckRotation,
			ObjectQueryParams,
			CheckShape,
			QueryParams
		);

	return !bVehicleExists;
}
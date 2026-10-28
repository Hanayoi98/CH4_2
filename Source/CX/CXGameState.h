#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CXGameState.generated.h"

class APlayerState;

UENUM(BlueprintType)
enum class ECXGameState : uint8
{
	Waiting,
	Playing,
	Finished
};

UCLASS()
class CX_API ACXGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ECXGameState GetCurrentState() const { return CurrentState; }
	int32 GetRemainingTime() const { return RemainingTime; }
	const TArray<TObjectPtr<APlayerState>>& GetWinners() const { return Winners; }

	bool IsWinner(const APlayerState* PlayerState) const;

	void SetCurrentState(ECXGameState NewState);
	void SetRemainingTime(int32 NewTime);
	void SetWinners(const TArray<APlayerState*>& NewWinners);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(Replicated)
	ECXGameState CurrentState = ECXGameState::Waiting;

	UPROPERTY(Replicated)
	int32 RemainingTime = 0;

	UPROPERTY(Replicated)
	TArray<TObjectPtr<APlayerState>> Winners;
};

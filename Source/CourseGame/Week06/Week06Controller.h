#pragma once
#include "CoreMinimal.h"
#include "Week05/CombatController.h"
#include "Week06Controller.generated.h"
class UStateCombatComponent;
UCLASS() class COURSEGAME_API AWeek06Controller:public ACombatController {
 GENERATED_BODY()
public:virtual void SetupInputComponent() override;virtual void OnPossess(APawn* InPawn) override;
 UPROPERTY() TObjectPtr<UStateCombatComponent> StateCombat;
 void StateFire();void HurtSelf();void KillSelf();void Feedback();
};

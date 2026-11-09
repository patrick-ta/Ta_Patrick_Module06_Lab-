#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Week06/StateRules.h"
#include "StateCombatComponent.generated.h"
class UPaperFlipbook;class UHealthComponent;class UBlasterComponent;
UCLASS(Blueprintable,ClassGroup=(Course),meta=(BlueprintSpawnableComponent))
class COURSEGAME_API UStateCombatComponent:public UActorComponent {
 GENERATED_BODY()
public:
 UStateCombatComponent();
 virtual void BeginPlay() override;
 virtual void TickComponent(float Delta,ELevelTick Type,FActorComponentTickFunction* Function) override;
 virtual void EndPlay(const EEndPlayReason::Type Reason) override;
 UFUNCTION(BlueprintCallable,Category="Week06") bool RequestAttack();
 UFUNCTION(BlueprintCallable,Category="Week06") void ToggleFeedback();
 UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Week06") bool bFeedback=true;
 UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Week06") bool bReducedEffects=false;
 UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Week06") TObjectPtr<UPaperFlipbook> Idle;
 UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Week06") TObjectPtr<UPaperFlipbook> Run;
 UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Week06") TObjectPtr<UPaperFlipbook> Air;
 UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Week06") TObjectPtr<UPaperFlipbook> Attack;
 UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Week06") TObjectPtr<UPaperFlipbook> Hurt;
 UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Week06") TObjectPtr<UPaperFlipbook> Dead;
 UPROPERTY(BlueprintReadOnly,Category="Week06") FString StateName=TEXT("Ready");
 UPROPERTY(BlueprintReadOnly,Category="Week06") int32 ShotAttempts=0;
 Week06::Model Rules;
private:
 UPROPERTY() TObjectPtr<UHealthComponent> Health;
 UPROPERTY() TObjectPtr<UBlasterComponent> Weapon;
 FTimerHandle FlashTimer;float PreviousHealth=100;
 UFUNCTION() void HealthChanged(float Current,float Maximum);
 UFUNCTION() void ResetFlash();
 void UpdateVisual();void PublishState(Week06::Action Before);
};

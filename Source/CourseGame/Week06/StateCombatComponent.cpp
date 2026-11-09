#include "Week06/StateCombatComponent.h"
#include "Week05/BlasterComponent.h"
#include "Week04/HealthComponent.h"
#include "Gameplay/RunnerCharacter.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"
UStateCombatComponent::UStateCombatComponent(){PrimaryComponentTick.bCanEverTick=true;}
void UStateCombatComponent::BeginPlay(){
 Super::BeginPlay();AddTickPrerequisiteActor(GetOwner());
 auto* R=Cast<ARunnerCharacter>(GetOwner());if(!R){SetComponentTickEnabled(false);return;}
 R->bExternalVisualSelector=true;Health=R->FindComponentByClass<UHealthComponent>();Weapon=R->FindComponentByClass<UBlasterComponent>();
 if(Health){PreviousHealth=Health->GetHealth();Health->OnHealthChanged.AddDynamic(this,&UStateCombatComponent::HealthChanged);}
 if(!Idle)Idle=R->IdleAnimation;if(!Run)Run=R->RunAnimation;
 if(!Air)Air=LoadObject<UPaperFlipbook>(nullptr,TEXT("/Game/Course/Week06/FB_Air.FB_Air"));
 if(!Attack)Attack=LoadObject<UPaperFlipbook>(nullptr,TEXT("/Game/Course/Week06/FB_Attack.FB_Attack"));
 if(!Hurt)Hurt=LoadObject<UPaperFlipbook>(nullptr,TEXT("/Game/Course/Week06/FB_Hurt.FB_Hurt"));
 if(!Dead)Dead=LoadObject<UPaperFlipbook>(nullptr,TEXT("/Game/Course/Week06/FB_Dead.FB_Dead"));
 ensureMsgf(Air&&Attack&&Hurt&&Dead,TEXT("Run Setup_Resources.ps1 before playing Module 6."));
}
void UStateCombatComponent::PublishState(Week06::Action Before){
 const TCHAR* Names[]={TEXT("Ready"),TEXT("Attacking"),TEXT("Hurt"),TEXT("Dead")};StateName=Names[int(Rules.State)];
 if(Before!=Rules.State)UE_LOG(LogTemp,Display,TEXT("W6_STATE %s -> %s generation=%llu"),Names[int(Before)],Names[int(Rules.State)],static_cast<unsigned long long>(Rules.Generation));
}
bool UStateCombatComponent::RequestAttack(){
 auto* R=Cast<ARunnerCharacter>(GetOwner());if(!R||!R->HasAuthority()||!R->bGameplayEnabled||!Health||Health->GetHealth()<=0)return false;
 auto Before=Rules.State;bool Accepted=Rules.Request(GetWorld()->GetTimeSeconds());PublishState(Before);
 UE_LOG(LogTemp,Display,TEXT("W6_REQUEST %s"),Accepted?TEXT("ACCEPTED_ACTION"):TEXT("REJECTED_ACTION"));return Accepted;
}
void UStateCombatComponent::TickComponent(float Delta,ELevelTick Type,FActorComponentTickFunction* Function){
 Super::TickComponent(Delta,Type,Function);auto Before=Rules.State;
 if(GetOwner()->HasAuthority()&&Rules.Step(GetWorld()->GetTimeSeconds())){
  ++ShotAttempts;const bool Fired=Weapon&&Weapon->TryFire();UE_LOG(LogTemp,Display,TEXT("W6_COMMIT attempt=%d spawned=%d"),ShotAttempts,int(Fired));
 }
 PublishState(Before);UpdateVisual();
}
void UStateCombatComponent::HealthChanged(float Current,float Maximum){
 if(Current>=PreviousHealth){PreviousHealth=Current;return;}PreviousHealth=Current;
 auto Before=Rules.State;Rules.Interrupt(GetWorld()->GetTimeSeconds(),Current<=0);PublishState(Before);
 auto* R=Cast<ARunnerCharacter>(GetOwner());if(Current<=0&&R)R->SetGameplayEnabled(false);
 GetWorld()->GetTimerManager().ClearTimer(FlashTimer);
 if(R&&bFeedback&&!bReducedEffects){R->GetSprite()->SetSpriteColor(FLinearColor(1,.35,.35,1));GetWorld()->GetTimerManager().SetTimer(FlashTimer,this,&UStateCombatComponent::ResetFlash,.08f,false);}
 else ResetFlash();
}
void UStateCombatComponent::ResetFlash(){if(auto* R=Cast<ARunnerCharacter>(GetOwner()))R->GetSprite()->SetSpriteColor(FLinearColor::White);}
void UStateCombatComponent::ToggleFeedback(){bFeedback=!bFeedback;GetWorld()->GetTimerManager().ClearTimer(FlashTimer);ResetFlash();}
void UStateCombatComponent::UpdateVisual(){
 auto* R=Cast<ARunnerCharacter>(GetOwner());if(!R)return;
 const auto V=Rules.Select(R->GetCharacterMovement()->IsFalling(),R->GetVelocity().X);
 UPaperFlipbook* Options[]={Idle.Get(),Run.Get(),Air.Get(),Attack.Get(),Hurt.Get(),Dead.Get()};UPaperFlipbook* Next=Options[int(V)];
 // LAB 5: replace/play a flipbook only when the desired asset changes.
 // TODO LAB 5: compare current asset before entry work{R->GetSprite()->SetFlipbook(Next);R->GetSprite()->SetLooping(V==Week06::Visual::Idle||V==Week06::Visual::Run);R->GetSprite()->PlayFromStart();}
 if (Next) {
	R->GetSprite()->SetFlipbook(Next);
	R->GetSprite()->SetLooping(V == Week06::Visual::Idle || V == Week06::Visual::Run);
	R->GetSprite()->PlayFromStart();
 }
}
void UStateCombatComponent::EndPlay(const EEndPlayReason::Type Reason){
 if(Health)Health->OnHealthChanged.RemoveDynamic(this,&UStateCombatComponent::HealthChanged);
 if(GetWorld())GetWorld()->GetTimerManager().ClearTimer(FlashTimer);
 ++Rules.Generation;Rules.State=Week06::Action::Dead;Super::EndPlay(Reason);
}

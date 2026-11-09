#include "Week06/Week06Controller.h"
#include "Week06/StateCombatComponent.h"
#include "Week04/HealthComponent.h"
#include "Components/InputComponent.h"
void AWeek06Controller::SetupInputComponent(){
 ARunnerController::SetupInputComponent(); // Skip old controller's direct-fire J binding.
 InputComponent->BindKey(EKeys::J,IE_Pressed,this,&AWeek06Controller::StateFire);
 InputComponent->BindKey(EKeys::H,IE_Pressed,this,&AWeek06Controller::HurtSelf);
 InputComponent->BindKey(EKeys::K,IE_Pressed,this,&AWeek06Controller::KillSelf);
 InputComponent->BindKey(EKeys::G,IE_Pressed,this,&AWeek06Controller::Feedback);
 InputComponent->BindKey(EKeys::T,IE_Pressed,this,&ACombatController::ResetRange);
 InputComponent->BindKey(EKeys::V,IE_Pressed,this,&ACombatController::ToggleSpeed);
}
void AWeek06Controller::OnPossess(APawn* P){Super::OnPossess(P);StateCombat=NewObject<UStateCombatComponent>(P);P->AddInstanceComponent(StateCombat);StateCombat->RegisterComponent();}
void AWeek06Controller::StateFire(){if(!bMenuOpen&&StateCombat)StateCombat->RequestAttack();}
void AWeek06Controller::HurtSelf(){if(GetPawn())if(auto* H=GetPawn()->FindComponentByClass<UHealthComponent>())H->ApplyDamage(10);}
void AWeek06Controller::KillSelf(){if(GetPawn())if(auto* H=GetPawn()->FindComponentByClass<UHealthComponent>())H->ApplyDamage(10000);}
void AWeek06Controller::Feedback(){if(StateCombat)StateCombat->ToggleFeedback();}

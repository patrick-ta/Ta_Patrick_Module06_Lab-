#include "Week06/Week06GameMode.h"
#include "Week06/Week06Controller.h"
AWeek06GameMode::AWeek06GameMode(){PlayerControllerClass=AWeek06Controller::StaticClass();DefaultPawnClass=LoadClass<APawn>(nullptr,TEXT("/Game/Course/Week03/BP_RunnerCharacter.BP_RunnerCharacter_C"));}

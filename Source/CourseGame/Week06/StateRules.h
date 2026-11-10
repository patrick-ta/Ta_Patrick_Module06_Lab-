#pragma once
#include <cmath>
#include <cstdint>
namespace Week06 {
enum class Action { Ready, Attacking, Hurt, Dead };
enum class Visual { Idle, Run, Air, Attack, Hurt, Dead };
struct Model {
 Action State=Action::Ready;
 double Started=0, LastTime=0;
 double Startup=.10, Active=.05, Recovery=.20, HurtTime=.30;
 std::uint64_t Generation=0;
 bool Committed=false;
 bool Clock(double now) { if(!std::isfinite(now)||now<0||now<LastTime)return false; LastTime=now;return true; }
 bool Request(double now) {
  if(!Clock(now)||State!=Action::Ready)return false;
  State=Action::Attacking;Started=now;Committed=false;++Generation;return true;
 }
 bool Step(double now) {
  if(!Clock(now))return false;
  bool fire=false;
  if(State==Action::Attacking) {
   const double elapsed=now-Started;
   // LAB 1: consume the active threshold once, including a frame crossing recovery.
   if(!Committed && elapsed>=Startup) {
    Committed=true;
    fire=true;
   }
   if(elapsed>=Startup+Active+Recovery)State=Action::Ready;
  } else if(State==Action::Hurt && now-Started>=HurtTime)State=Action::Ready;
  return fire;
 }
 void Interrupt(double now,bool dead) {
  if(!Clock(now)||State==Action::Dead)return;
  // LAB 2: invalidate old work and commit Hurt or terminal Dead.
  ++Generation;                              
  Started=now;                              
  State=dead?Action::Dead:Action::Hurt;
 }
 bool Complete(std::uint64_t token,double now) {
  // LAB 3: stale/dead completions must leave state and clock untouched.
  if(token!=Generation||State!=Action::Hurt)return false;   
  if(!std::isfinite(now)||now<0||now<LastTime)return false;
  if(now-Started<HurtTime)return false;  
  if(!Clock(now))return false;State=Action::Ready;return true;
 }
 Visual Select(bool falling,double speed) const {
  // LAB 4: visual selection reads action, grounding and speed without mutating state.
  if(State==Action::Dead)return Visual::Dead;
  if(State==Action::Hurt)return Visual::Hurt;
  if(State==Action::Attacking)return Visual::Attack;
  if(falling)return Visual::Air;
  if(std::fabs(speed)>5.0)return Visual::Run;
  return Visual::Idle;
 }
};
inline bool ChangeVisual(int current,int desired) { return current!=desired; }
}

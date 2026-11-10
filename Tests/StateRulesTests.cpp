#include "StateRules.h"
#include <iostream>
#include <limits>
int main(int argc,char**argv){
 using namespace Week06;int total=0,failed=0;
 auto check=[&](const char*n,bool ok){++total;if(!ok)++failed;std::cout<<n<<","<<(ok?"PASS":"FAIL")<<"\n";};
 if(argc>1){Model m;check("starter request",m.Request(0));check("starter intentionally cannot commit",!m.Step(.1));check("starter placeholder visual",m.Select(false,0)==Visual::Idle);return failed?1:0;}
 Model m;check("first request",m.Request(0));check("rapid repeat",!m.Request(.01));
 check("before startup",!m.Step(.099));check("at startup",m.Step(.10));check("one commit",!m.Step(.11));
 check("still locked",!m.Request(.12));check("recovery before end",!m.Step(.349)&&m.State==Action::Attacking);
 check("ready after recovery",!m.Step(.351)&&m.State==Action::Ready);check("new request",m.Request(.4));
 m.Interrupt(.45,false);auto old=m.Generation;check("hurt state",m.State==Action::Hurt);check("hurt rejects request",!m.Request(.46));check("cancel startup",!m.Step(.55));
 check("early hurt completion",!m.Complete(old,.60));check("hurt expires",!m.Step(.751)&&m.State==Action::Ready);
 m.Request(.8);auto token=m.Generation;m.Interrupt(.81,true);check("dead state",m.State==Action::Dead);check("dead rejects request",!m.Request(.9));
 check("stale callback",!m.Complete(token,1.5)&&m.State==Action::Dead);check("death terminal",!m.Step(2)&&m.State==Action::Dead);
 Model low;low.Request(0);check("low FPS crosses entire action",low.Step(.50)&&low.State==Action::Ready);check("low FPS once",!low.Step(.60));
 Model same;same.Request(0);same.Interrupt(.1,false);check("hurt before active cancels",!same.Step(.1));
 Model after;after.Request(0);check("active first commits",after.Step(.1));after.Interrupt(.1,false);check("hurt after commit cannot duplicate",!after.Step(.2));
 Model v;check("idle",v.Select(false,0)==Visual::Idle);check("run",v.Select(false,6)==Visual::Run);check("air",v.Select(true,20)==Visual::Air);
 v.Request(0);check("attack priority",v.Select(true,20)==Visual::Attack);v.Interrupt(.1,false);check("hurt priority",v.Select(true,20)==Visual::Hurt);v.Interrupt(.2,true);check("dead priority",v.Select(true,20)==Visual::Dead);
 check("same visual no restart",!ChangeVisual(2,2));check("new visual changes",ChangeVisual(2,3));
 Model invalid;check("nan rejected",!invalid.Request(std::numeric_limits<double>::quiet_NaN()));check("negative rejected",!invalid.Request(-1));invalid.Request(1);check("backward time rejected",!invalid.Step(.5));
 Model cancel;cancel.Request(0);auto stale=cancel.Generation;cancel.Interrupt(.01,false);auto clock=cancel.LastTime;check("stale leaves clock",!cancel.Complete(stale,100)&&cancel.LastTime==clock);
 Model a,b;a.Request(0);b.Request(0);bool fa=a.Step(.11),fb=b.Step(.11);(void)a.Select(false,0);check("presentation does not change rules",fa==fb&&a.State==b.State&&a.Generation==b.Generation);
 std::cout<<"TOTAL,"<<total<<",FAILED,"<<failed<<"\n";return failed?1:0;
}

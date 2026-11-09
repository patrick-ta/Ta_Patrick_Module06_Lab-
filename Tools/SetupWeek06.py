import unreal as u
from pathlib import Path
assets=u.EditorAssetLibrary
for name,source,fps in [('Air','FB_PlayerIdle',3),('Attack','FB_PlayerRun',18),('Hurt','FB_PlayerIdle',5),('Dead','FB_PlayerIdle',1)]:
 target='/Game/Course/Week06/FB_'+name
 if not assets.does_asset_exist(target):
  asset=assets.duplicate_asset('/Game/Course/Characters/'+source,target)
  if not asset:raise RuntimeError('Could not create '+target)
  asset.set_editor_property('frames_per_second',fps)
  assets.save_loaded_asset(asset)
world=u.EditorLoadingAndSavingUtils.load_map('/Game/Course/Maps/L_Week05')
if not world:raise RuntimeError('Missing supplied range map')
world.get_world_settings().set_editor_property('default_game_mode',u.load_class(None,'/Script/CourseGame.Week06GameMode'))
u.EditorLoadingAndSavingUtils.save_map(world,'/Game/Course/Maps/L_Week06')
for name in ['Air','Attack','Hurt','Dead']:
 if not assets.does_asset_exist('/Game/Course/Week06/FB_'+name):raise RuntimeError('Missing '+name)
Path(u.Paths.project_dir(),'RESOURCES_READY.flag').write_text('Setup completed in Unreal. Run the play checklist next.\n')
u.log('WEEK06_RESOURCES_READY')

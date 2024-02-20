#!/usr/bin/env bash

/mnt/storage/unreal_editors/Linux_Unreal_Engine_5.3.2/Engine/Build/BatchFiles/Linux/Build.sh -mode=GenerateClangDatabase \
      -project=/home/niscolas/bonfire/projects/MonoGrinding/MonoGrinding.uproject \
      MonoGrindingEditor Development Linux \
      -OutputDir=/home/niscolas/bonfire/projects/MonoGrinding/

# /mnt/storage/unreal_editors/Linux_Unreal_Engine_5.3.2/Engine/Build/BatchFiles/Linux/Build.sh MonoGrinding Linux /home/niscolas/bonfire/projects/MonoGrinding/MonoGrinding.uproject -VSCode

#!/bin/bash

#arg 1 => name of the scene
#arg 2 => folder has to be created, has to exist!

cd "$(dirname "$0")"
cd ..
cd "$2"

templatesDir="../generator/templates"

scenename="$1Scene"

name="$scenename"
view="View"
model="Model"
controller="Controller"

view="$name$view"
model="$name$model"
controller="$name$controller"

viewfolder="view"
modelfolder="model"
controllerfolder="controller"

#remove foders if they exist
rm -rf "$viewfolder"
rm -rf "$modelfolder"
rm -rf "$controllerfolder"

#create directories
mkdir "$viewfolder"
mkdir "$modelfolder"
mkdir "$controllerfolder"

#files in folders
scene="$name.hpp"
view="$viewfolder/$view.hpp"
model="$modelfolder/$model.hpp"
controller="./$controllerfolder/$controller.hpp"

#create files
touch "$scene"
touch "$view"
touch "$model"
touch "$controller"

#placeholders to replace
placeholder="SCENE_NAME"

#fill files and replace placeholders

#controller
cat "$templatesDir"/ControllerTemplate > "$controller"
sed -i '' 's/'$placeholder'/'$name'/g' "$controller" 

#model
cat "$templatesDir"/ModelTemplate > "$model"
sed -i '' 's/'$placeholder'/'$name'/g' "$model" 

#view
cat "$templatesDir"/ViewTemplate > "$view"
sed -i '' 's/'$placeholder'/'$name'/g' "$view" 

#scene
cat "$templatesDir"/SceneTemplate > "$scene"
sed -i '' 's/'$placeholder'/'$name'/g' "$scene" 

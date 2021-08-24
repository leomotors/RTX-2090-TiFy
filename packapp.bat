REM This Batch File is used for assist in Packing Windows Version
REM This will be run after building with Visual Studio

set project_ver=%2
set dist_name="RTX-2090-TiFy-%project_ver%-Windows"

mkdir %dist_name%
mkdir %dist_name%\assets
mkdir %dist_name%\exports
mkdir %dist_name%\temp

set project_root_dir=%1
set project_root_dir=%project_root_dir:/=\%

copy %project_root_dir%\assets\ %dist_name%\assets
copy *.exe %dist_name%
copy *.dll %dist_name%
copy *.ilk %dist_name%
copy *.pdb %dist_name%

echo PACKING COMPLETED, Openning File Explorer...
explorer.exe .

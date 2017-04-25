#!/bin/bash
pause() {
 read -rp $'Press any key to continue or Ctrl+C to abort . . .\n'
}

echo ""
echo "This script updates the Stanford CPP lib used in CodeStepByStep."
pause

echo "Updating . . ."

# copy stanfordcpplib.cpp/h to CodeStepByStep project and jail root dir
cp stanfordcpplib.* ~/codestepbystep/web/languages/cpp/
sudo cp stanfordcpplib.* /var/chroot-jail/tmp/codestepbystep-jail/
sudo rm /var/chroot-jail/tmp/codestepbystep-jail/*.o 2>/dev/null

# copy spl.jar to CodeStepByStep project and jail root dir
sudo cp JavaBackEnd/eclipseproject/obf/spl.jar ~/codestepbystep/web/WEB-INF/lib/spl.jar 
sudo cp JavaBackEnd/eclipseproject/obf/spl.jar /var/chroot-jail/tmp/codestepbystep-jail/spl.jar 
echo "Complete."
echo ""

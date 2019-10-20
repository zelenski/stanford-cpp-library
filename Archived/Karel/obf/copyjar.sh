#!/bin/bash
echo ""
echo "Copying karel.jar to ~/106a ..."
for JAR in `find ~/106a/ -name karel.jar`; do cp karel.jar $JAR; done
echo "Complete."

#! /bin/sh
sudo make clean
make nanvix -j 8
make image
file=/home/leo/Documents/linux/Cours_4A/s8/AS/nanvix/hdd.img.lock
if [ -e "$file" ]; then
    rm -rf "$file" || echo "rm done"
else echo "rm not done"
fi
konsole -e bash tools/run/run.sh&
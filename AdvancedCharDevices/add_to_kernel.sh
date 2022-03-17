sudo mount -t ext2 /home/maher/kernel/buildroot/output/images/rootfs.ext2 /home/maher/kernel/buildroot/output/images/mount
for var in "$@"
do
	cp $var /home/maher/kernel/buildroot/output/images/mount
done
sudo umount /home/maher/kernel/buildroot/output/images/mount
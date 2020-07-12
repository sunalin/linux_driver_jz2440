
ifconfig  eth0 192.168.31.201 netmask 255.255.255.0
route add default gw 192.168.31.1

ifconfig
ip route show

mkdir /server_file_mnt
mount -o nolock,rw 192.168.31.200:/server_file /server_file_mnt
#umount /server_file_mnt
#umount -f /server_file_mnt
#fuser -m /server_file_mnt
#fuser -k /server_file_mnt



# master host open 'nfs' server:
#vim /etc/exports
#/server_file  *(rw,sync,no_root_squash)
#:wq
#/etc/init.d/nfs-kernel-server restart


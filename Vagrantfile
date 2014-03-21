# -*- mode: ruby -*-
# vi: set ft=ruby :

VAGRANTFILE_API_VERSION = "2"

Vagrant.configure(VAGRANTFILE_API_VERSION) do |config|

  config.vm.box = "ubuntu-12.04-32bit"
  config.vm.box_url = "http://files.vagrantup.com/precise32.box"
  config.vm.synced_folder "./", "/home/vagrant/ardupilot"
  
  # Forward the SITL port to the host to use in ground stations
  config.vm.network "forwarded_port", guest: 5760, host: 5760

  # And, if MAVProxy is in the loop, forward from its MAVLink repeater...
  config.vm.network "forwarded_port", guest: 14550, host: 14550

  config.vm.provider :virtualbox do |vb|

    # Otherwise the compile will go into swap, making things slow
    vb.memory = 3072
    vb.cpus = 2

  end

  config.vm.provision "chef_solo" do |chef|
    chef.add_recipe "ardupilot"
  end

  # Allow forwarding of X sessions for --map and other MAVProxy plugin options.
  config.ssh.forward_x11 = true

  # Allow symlinks... used at all?
  # config.vm.customize ["setextradata", :id, "VBoxInternal2/SharedFoldersEnableSymlinksCreate/cross-compiler", "1"]
    
end
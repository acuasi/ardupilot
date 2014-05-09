# -*- mode: ruby -*-
# vi: set ft=ruby :

VAGRANTFILE_API_VERSION = "2"

Vagrant.configure(VAGRANTFILE_API_VERSION) do |config|

  config.vm.box = "ubuntu-12.04-32bit"
  config.vm.box_url = "http://files.vagrantup.com/precise32.box"
  config.vm.synced_folder "./", "/home/vagrant/ardupilot"

  # Static IP makes connecting host machine to guest VM easier, but edit if needed.
  config.vm.network "private_network", ip: "192.168.50.4"

  config.vm.provider :virtualbox do |vb|

  # The VM needs decent resources to run the simulations effectively.
  # Modify as required on your local setup.  If the autotests are failing,
  # increase memory and/or available CPU cores as much as possible
  # to see if the simulations are causing problems.
  vb.memory = 4096
  vb.cpus = 4

  end

  config.vm.provision "chef_solo" do |chef|
    chef.add_recipe "ardupilot"
  end

  # Allow forwarding of X sessions for --map and other MAVProxy plugin options.
  config.ssh.forward_x11 = true

end
# System packages needed to install software.
include_recipe "apt"

[
  # Core/common dependencies
  'make',
  'git',
  'arduino-core',
  'curl',
  'gawk',
  'g++',
  'dos2unix',
  'ccache',

  # PX4 deps here
  'python-serial',
  'openocd',
  'flex',
  'bison',
  'libncurses5-dev',
  'autoconf',
  'texinfo',
  'build-essential',
  'libftdi-dev',
  'libtool',
  'zlib1g-dev',
  'zip',
  'genromfs' ,
  
  # Some Python dependencies are easier to build this way than with pip.
  'python-opencv',
  'python-wxgtk2.8',
  'python-matplotlib',
  'xterm',
  ].each do |pkg|
  package pkg
end

# Python dependencies
include_recipe "python"
python_pip "pexpect"
python_pip "pymavlink"
python_pip "mavproxy"
python_pip "pillow"

# Trigger initial generation of a config.mk file.
execute "cd /home/vagrant/ardupilot/ArduCopter && make configure"

# Manage the .profile to add paths & configure the shell.
template "/home/vagrant/.profile" do
  source "profile.erb"
  mode 0644
  owner "vagrant"
  group "vagrant"
end

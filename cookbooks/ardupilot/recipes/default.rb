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

# Trigger initial generation of a config.mk file.
execute "cd /home/vagrant/ardupilot/ArduCopter && make configure"

# Additional repositories to support development.
git "/home/vagrant/mavproxy" do
  repository "https://github.com/tridge/MAVProxy"
  action :sync
end
execute "cd /home/vagrant/mavproxy && sudo easy_install ."

git "/home/vagrant/mavlink" do
  repository "https://github.com/mavlink/mavlink"
  action :sync
end
execute "cd /home/vagrant/mavlink/pymavlink && sudo easy_install ."

# Manage the .profile to add paths & configure the shell.
template "/home/vagrant/.profile" do
  source "profile.erb"
  mode 0644
  owner "vagrant"
  group "vagrant"
end

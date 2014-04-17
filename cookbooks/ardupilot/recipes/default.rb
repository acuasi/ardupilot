# System packages needed to install software.
include_recipe "apt"

[
  'make',
  'git',
  'arduino-core',
  'curl',
  'gawk',
  'g++',
  'dos2unix',
  'ccache',
  'python-serial',
  'openocd',
  'flex',
  'bison',
  'libncurses5-dev',
  'autoconf',
  'automake',
  'libexpat1-dev',
  'texinfo',
  'build-essential',
  'libftdi-dev',
  'libtool',
  'zlib1g-dev',
  'zip',
  'genromfs' ,
  
  # Some Python dependencies are easier to build this way than with pip.
  'python-opencv',
  'python-scipy',
  'python-wxgtk2.8',
  'python-matplotlib',
  'python-serial',
  'xterm',
  ].each do |pkg|
  package pkg
end

git "JSBSim" do
    repository "https://github.com/tridge/jsbsim.git"
    user "vagrant"
    group "vagrant"
    destination "/home/vagrant/JSBSim"
    revision "master"
    action :sync
end

# The symlink below is needed when this is run through pexpect.
# The pexpect.spawn invokes os.execv() which doesn't search the path
# correctly.
bash "install_jsbsim" do
  cwd "/home/vagrant/JSBSim"
  user "vagrant"
  group "vagrant"
  code <<-EOH
    ./autogen.sh
    make
  EOH
end

bash "symlink_jbsim" do
  code <<-EOH
      ln -s /home/vagrant/JSBSim/src/JSBSim /bin/JSBSim
  EOH
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

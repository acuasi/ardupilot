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
python_pip "pymavlink"
python_pip "mavproxy"
python_pip "pexpect"

# Trigger initial generation of a config.mk file.
execute "cd /home/vagrant/ardupilot/ArduCopter && make configure"

# Environmental modifications
ruby_block "edit-bashrc-environment" do
  block do
    file = Chef::Util::FileEdit.new("#{ENV['HOME']}/.bashrc")
    file.insert_line_if_no_match(
      "# Include paths needed to run Arudpilot code and tests",
      "\n\n# Include paths needed to run Arudpilot code and tests\nexport PATH=/usr/lib/ccache:$PATH"
    )
    file.write_file
  end
end





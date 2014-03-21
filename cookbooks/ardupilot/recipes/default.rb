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

  # SITL/autotest dependencies here
  'dos2unix',
  'ccache',
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

command "cd ardupilot/ArduCopter && make configure"

# Environmental modifications
# ruby_block "include-bashrc-user" do
#   block do
#     file = Chef::Util::FileEdit.new("#{ENV['HOME']}/.bashrc")
#     file.insert_line_if_no_match(
#       "# Include paths needed to run Arudpilot code/tests\n",
#       "export PATH=/usr/lib/ccache:$PATH\n",
#       "export PATH=$PATH:$HOME/GitHub/ardupilot/Tools/autotest\n",
#       "export PATH=$PATH:$HOME/GitHub/mavlink/pymavlink\n",
#       "export PATH=$PATH:$HOME/GitHub/MAVProxy\n"
#     )
#     file.write_file
#   end
# end





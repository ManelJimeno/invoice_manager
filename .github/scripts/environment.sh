#!/bin/bash
set -e

virtual=$1
venv_path=$2
python_exec=$3
pip_install_path=$4
build_dir=$5
build_type=$6

if [ "$virtual" = "true" ]; then
  if [ "$RUNNER_OS" = "Windows" ]; then
    unix_path=$(echo "$venv_path" | sed 's/\\/\//g')
    "${unix_path}"/Scripts/activate
  else
    source ${venv_path}/bin/activate
  fi
  python -c "import sys; print('Python virtual environment activation')"
  python -c "import sys; print('Using: ', sys.executable, sys.version)"
else
  python_path=$(dirname ${python_exec})
  echo "$pip_install_path" >> "$GITHUB_PATH"
  echo "$python_path" >> "$GITHUB_PATH"
  export PATH=$PATH:"$pip_install_path":"$python_path"
#  if [ "$RUNNER_OS" = 'Linux' ] || [ "$RUNNER_OS" = 'macOS' ]; then
#    echo "$HOME/.local/bin" >> "$GITHUB_PATH"
#    export PATH=$PATH:$HOME/.local/bin
#  elif [ "$RUNNER_OS" = 'Windows' ]; then
#    parent_directory=$(dirname "$(which python)")
#    echo "$parentDirectory/Scripts" >> "$GITHUB_PATH"
#    export PATH=$PATH:$parent_directory
#  fi
fi

if [ "$RUNNER_OS" = "Windows" ]; then
  file="./"build_dir"/conan/build/generators/conanrun.sh"
  if [ -e "$file" ]; then
    sed -i 's/\\/\//g' $file
  fi
else
  file=""build_dir"/conan/build/"build_type"/generators/conanrun.sh"
fi
echo "Conan environment setting script path: $file"

if [ -e "$file" ]; then
  source $file
fi

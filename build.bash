# pretty text awaits
RED=`tput setaf 1`
GRN=`tput setaf 2`
YEL=`tput setaf 3`
BLU=`tput setaf 4`
MAG=`tput setaf 5`
CYN=`tput setaf 6`
RST=`tput sgr0`

#
# additional project folders specified here:
#
SRC_FOLDERS=( \
    lib lib/modules lib/cpp-api lib/modules/homebrew lib/generate \
    lib/modules/adders lib/modules/riscv )

COMPILER="g++"
STDOPTS="-I. -std=c++11 -O3"
LINKOPTS=""
ALL_OBJS=""

if [[ $1 == "--help" ]]; then
    printf "${YEL}options:${RST}\n"
    printf "    ${BLU}--help${RST}     -  print this help text\n"
    printf "    ${BLU}--release${RST}  -  generate Makefile with standard compile options\n"
    printf "    ${BLU}--asan${RST}     -  generate Makefile with debug options enabled (-g, -fsanitize=address)\n"
    printf "    ${BLU}--valgrind${RST} -  generate Makefile with Valgrind-compat options enabled (-g, -DTRACE_ON_EXIT)\n"
    exit 0
elif [[ $1 == "--asan" ]]; then

    printf "\n${MAG}Generating Makefile with ${GRN}ASAN${RST} options enabled\n\n"
    STDOPTS="-I. -std=c++11 -O3 -g -fsanitize=address"

elif [[ $1 == "--valgrind" ]]; then

    printf "\n${MAG}Generating Makefile with debug options compatible with ${GRN}Valgrind${RST}\n\n"
    STDOPTS="-I. -std=c++11 -O0 -DTRACE_ON_EXIT -g"

elif [[ $1 == "--release" ]]; then

    printf "\n${MAG}Generating Makefile with standard build options enabled${RST}\n\n"

else
    printf "\nrun build script with option ${BLU}--help${RST} to see available options\n\n"
    exit 0
fi

function gen_build_for {

    local cur_dir=${1}

    mkdir -p obj/$cur_dir

    files_in_dir=( `ls $cur_dir | grep -E "\.c(pp)?$"` )
    for fname in "${files_in_dir[@]}"; do

        local base_name="${fname%%.*}"
        local obj_file="obj/$cur_dir/$base_name.o"
        local src_file="$cur_dir/$fname"
        local header_file="$cur_dir/$base_name.h"

        echo "${CYN}Generating build cmd for ${YEL}$cur_dir/$fname ${RST}"
        
        ALL_OBJS="$obj_file $ALL_OBJS"

        echo "$obj_file: $src_file $header_file" >> Makefile
        printf "\t$COMPILER -c $STDOPTS -o $obj_file $src_file\n\n" >> Makefile

    done
}

printf "\n# this file was automatically generated\n\n" > Makefile

echo "all: main" >> Makefile
echo "" >> Makefile

for dirname in "${SRC_FOLDERS[@]}"; do gen_build_for $dirname; done

echo "main: main.cpp $ALL_OBJS" >> Makefile
printf "\t$COMPILER $STDOPTS $LINKOPTS -o main main.cpp $ALL_OBJS\n" >> Makefile
echo "" >> Makefile

echo "clean:" >> Makefile
printf "\trm $ALL_OBJS\n" >> Makefile
echo "" >> Makefile

if [[ $1 == "--valgrind" ]]; then
    echo "run:" >> Makefile
    printf "\tvalgrind -s --leak-check=yes ./main\n\n" >> Makefile
else
    echo "run:" >> Makefile
    printf "\t./main\n\n" >> Makefile
fi

printf "\n    to run program: '${GRN}make${RST}' and '${GRN}make run${RST}'\n\n"

echo "wHy NoT jUsT uSe cMaKe!?"
echo "because i dont want to"
